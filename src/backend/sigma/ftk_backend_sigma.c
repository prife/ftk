#include "ftk_log.h"
#include "ftk_globals.h"
#include "ftk_wnd_manager.h"
#include "ftk_display_sigma.h"
#define ALLOW_OS_CODE 1
#include "common.h"
#include "sample_os.h"
#include "dcc/include/dcc.h"

static Ret sigma_init(int argc, char* argv[])
{
	struct DCC *pDCC = NULL;
	struct RUA *pInstance = NULL;
	RMuint32 colorbars;
	RMbool enable;
	RMstatus err;
	static struct dcc_context dcc_info;
	struct dh_context dh_info;
	struct display_cmdline disp_opt;
	struct display_context disp_info;
	struct audio_cmdline audio_opt;
	struct playback_cmdline play_opt;
	void **dmabuffer_array;
	RMuint32 dmabuffer_index;
	struct RUABufferPool *pDMA;
	RMuint8 *buf = NULL;
	
	RMMemset(&dcc_info, 0, sizeof(dcc_info));
	RMMemset(&disp_info, 0, sizeof(disp_info));
	RMMemset(&dh_info, 0, sizeof(dh_info));
	init_display_options(&disp_opt);
	init_audio_options(&audio_opt);
	init_playback_options(&play_opt);
	disp_opt.dh_info = &dh_info;
	audio_opt.dh_info = &dh_info;
	
	if (RMFAILED(err = parse_cmdline(argc, argv, &disp_opt, &play_opt, &audio_opt))) {
		RMDBGLOG((ENABLE, "Error parsing command line! %d\n", err));
		return -1;
	}
	
	if (RMFAILED(err = RUACreateInstance(&pInstance, chip_num))) {
		RMDBGLOG((ENABLE, "Error creating instance! %d\n", err));
		return -1;
	}
	
	err = DCCOpen(pInstance, &pDCC);
	if (RMFAILED(err)) {
		RMDBGLOG((ENABLE, "Error Opening DCC! %d\n", err));
		return -1;
	}
	
	if (audio_generate) {
		err = DCCInitMicroCodeEx(pDCC, disp_opt.init_mode);
	} else {
		err = DCCInitChainEx(pDCC, disp_opt.init_mode);
	}
	if (RMFAILED(err)) {
		RMDBGLOG((ENABLE, "Cannot initialize microcode %d\n", err));
		return -1;
	}
	
	dcc_info.chip_num = chip_num;
	dcc_info.pRUA = pInstance;
	dcc_info.pDCC = pDCC;
	dcc_info.SurfaceID = 0;
	dcc_info.pVideoSource = NULL;
	dcc_info.pMultipleAudioSource = NULL;
	dcc_info.pStcSource = NULL;
	dcc_info.state = RM_PLAYING;
	dcc_info.route = DCCRoute_ColorBars;
	dcc_info.disp_info = &disp_info;
	dcc_info.dh_info = &dh_info;
	dcc_info.trickmode_id = RM_NO_TRICKMODE;
	dcc_info.seek_supported = FALSE;
	
	colorbars = EMHWLIB_MODULE(DispColorBars, 0);
	
	while ((err = RUASetProperty(pInstance, colorbars, RMDispColorBarsPropertyID_Intensity, &intensity, sizeof(intensity), 0)) == RM_PENDING);
	if (err != RM_OK) {
		RMDBGLOG((ENABLE, "Cannot set color bars intensity, %d\n", err));
		return -1;
	}
	
	if (! standard) switch (disp_opt.standard) {
	case EMhwlibTVStandard_PAL_60:
	case EMhwlibTVStandard_PAL_M:
	case EMhwlibTVStandard_PAL_60_714:
	case EMhwlibTVStandard_PAL_M_714:
	case EMhwlibTVStandard_ITU_Bt656_625:
	case EMhwlibTVStandard_ITU_Bt656_288p:
	case EMhwlibTVStandard_PAL_BG:
	case EMhwlibTVStandard_PAL_N:
	case EMhwlibTVStandard_576p50:
	case EMhwlibTVStandard_PAL_BG_702:
	case EMhwlibTVStandard_PAL_N_702:
	case EMhwlibTVStandard_576p50_702:
	case EMhwlibTVStandard_PAL_BG_704:
	case EMhwlibTVStandard_PAL_N_704:
	case EMhwlibTVStandard_576p50_704:
		standard = EMhwlibColorBarsStandard_PAL;
		break;
	case EMhwlibTVStandard_ITU_Bt656_525:
	case EMhwlibTVStandard_ITU_Bt656_240p:
	case EMhwlibTVStandard_NTSC_M_Japan:
	case EMhwlibTVStandard_NTSC_M:
	case EMhwlibTVStandard_480p59:
	case EMhwlibTVStandard_NTSC_M_Japan_714:
	case EMhwlibTVStandard_NTSC_M_714:
	case EMhwlibTVStandard_480p59_714:
	default:
		standard = EMhwlibColorBarsStandard_NTSC;
		break;
	}
	while ((err = RUASetProperty(pInstance, colorbars, RMDispColorBarsPropertyID_Standard, &standard, sizeof(standard), 0)) == RM_PENDING);
	if (err != RM_OK) {
		RMDBGLOG((ENABLE, "Cannot set color bars standard, %d\n", err));
		return -1;
	}
	
	while ((err = RUASetProperty(pInstance, colorbars, RMGenericPropertyID_Validate, NULL, 0, 0)) == RM_PENDING);
	if (err != RM_OK) {
		RMDBGLOG((ENABLE, "Cannot validate color bars properties, %d\n", err));
		return -1;
	}
	
	enable = TRUE;
	while ((err = RUASetProperty(pInstance, colorbars, RMGenericPropertyID_Enable, &enable, sizeof(enable), 0)) == RM_PENDING);
	if (err != RM_OK) {
		RMDBGLOG((ENABLE, "Cannot enable color bars, %d\n", err));
		return -1;
	}	
	
	err = apply_display_options(&dcc_info, &disp_opt);
	if (RMFAILED(err)) {
		RMDBGLOG((ENABLE, "Cannot set display opions %d\n", err));
		return -1;
	}
	
	if (audio_generate) {
		struct DCCAudioProfile audio_profile = {0,};
		
		if (! dcc_info.pStcSource) {
			struct DCCStcProfile stc_profile;
			
			// open first stc module
			stc_profile.STCID = play_opt.STCid;
			stc_profile.master = Master_STC;
			
			stc_profile.stc_timer_id = 3 * play_opt.STCid;
			stc_profile.stc_time_resolution = 90000;
			
			stc_profile.video_timer_id = 3 * play_opt.STCid + 1;
			stc_profile.video_time_resolution = 90000;
			stc_profile.video_offset = 0;
			
			stc_profile.audio_timer_id = 3 * play_opt.STCid + 2;
			stc_profile.audio_time_resolution = 90000;
			stc_profile.audio_offset = 0;
			
			err = DCCSTCOpen(dcc_info.pDCC, &stc_profile, &dcc_info.pStcSource);
			if (RMFAILED(err)) {
				fprintf(stderr, "Cannot open stc module %d\n", err);
				return RM_ERROR;
			}
		}
		
		DCCSTCSetTime(dcc_info.pStcSource, 0, 90000);
		DCCSTCPlay(dcc_info.pStcSource);
		
		audio_opt.Codec = AudioDecoder_Codec_PCM;
		audio_opt.SubCodec = 0;
		audio_opt.PcmCdaParams.ChannelAssign = PcmCda2_LR;
		audio_opt.PcmCdaParams.BitsPerSample = 16;
		//audio_opt.PcmCdaParams.MsbFirst = FALSE;
		//audio_opt.sync_stc = FALSE;
		
		if (! audio_opt.fifo_size) audio_opt.fifo_size = AUDIO_FIFO_SIZE;
		if (! audio_opt.xfer_count) audio_opt.xfer_count = XFER_FIFO_COUNT;
		if (! play_opt.dmapool_count) play_opt.dmapool_count = DMA_BUFFER_COUNT;
		if (! play_opt.dmapool_log2size) play_opt.dmapool_log2size = DMA_BUFFER_SIZE_LOG2;
		dmabuffer_array = (void **)RMMalloc(sizeof(void*) * play_opt.dmapool_count);
		dmabuffer_index = 0;
		if (dmabuffer_array == NULL) {
			RMDBGLOG((ENABLE, "Cannot allocate dmapool array! Disable disk control\n"));
		}
		
		err = apply_playback_options(&dcc_info, &play_opt);
		if (RMFAILED(err)) {
			fprintf(stderr, "Cannot set playback options %d\n", err);
			return -1;
		}
		
		// Open AudioDecoder
		audio_profile.BitstreamFIFOSize = audio_opt.fifo_size;
		audio_profile.XferFIFOCount = audio_opt.xfer_count;
		audio_profile.DemuxProgramID = audio_opt.AudioEngineID * 2;
		audio_profile.AudioEngineID = audio_opt.AudioEngineID;
		audio_profile.AudioDecoderID = audio_opt.AudioDecoderID;
		audio_profile.STCID = play_opt.STCid;
		err = DCCOpenMultipleAudioDecoderSource(dcc_info.pDCC, &audio_profile, 1, &(dcc_info.pMultipleAudioSource));
		if (RMFAILED(err)) {
			fprintf(stderr, "Cannot open audio decoder! %s\n", RMstatusToString(err));
			return err;
		}
		
		//err = DCCGetAudioDecoderSourceInfo(dcc_info.pAudioSource, &(dcc_info.audio_decoder), &(dcc_info.audio_engine), &(dcc_info.audio_timer));
		//if (RMFAILED(err)) {
		//	fprintf(stderr, "Error getting audio decoder source information! %s\n", RMstatusToString(err));
		//	return err;
		//}
		
		// apply the sample rate, serial out status
		err = apply_audio_engine_options(&dcc_info, &audio_opt);
		if (RMFAILED(err)) {
			fprintf(stderr, "Error applying audio engine options! %s\n", RMstatusToString(err));
			return err;
		}
		
		/* dmapool must be created after the module open in case we do no copy transfers */
		err = RUAOpenPool(dcc_info.pRUA, 0, play_opt.dmapool_count, play_opt.dmapool_log2size, RUA_POOL_DIRECTION_SEND, &pDMA);
		
		// apply the audio format - uninit, set codec, set specific parameters, init
		err = apply_audio_decoder_options(&dcc_info, &audio_opt);
		if (RMFAILED(err)) {
			fprintf(stderr, "Error applying audio_decoder_options! %s\n", RMstatusToString(err));
			return err;
		}
		
		// Send Play command to audio playback
		err = DCCPlayMultipleAudioSource(dcc_info.pMultipleAudioSource);
		if (RMFAILED(err)) {
			fprintf(stderr, "Cannot play audio decoder! %s\n", RMstatusToString(err));
			return err;
		}
		
		apply_dvi_hdmi_audio_options(&dcc_info, &audio_opt, 0, FALSE, FALSE, FALSE);
	}
	
	RMDBGPRINT((ENABLE, "-i <0|1>: Set the intensity level [0]=75%%, 1=100%%\n"));
	display_key_usage(KEYFLAGS);
	
	RMTermInit(TRUE);    // don't allow ctrl-C and the like ...
	RMSignalInit(NULL, NULL);  // ... but catch other termination signals to call RMTermExit()
	do {
		RMuint32 cmd;
		err = process_key(&dcc_info, &cmd, KEYFLAGS);
		if (RMFAILED(err)) {
			RMDBGLOG((ENABLE, "Error while processing key %d\n", err));
			break;
		}
		switch(cmd) {
		case RM_QUIT:
		case RM_STOP:
			goto cleanup;
			break;
		default:
			RMMicroSecondSleep(50*1000);
			break;
		}
		if (audio_generate) {
			RMuint32 count;
			RMstatus status = RM_OK;
			struct emhwlib_info Info;
			struct emhwlib_info *pInfo = &Info;
			RMuint32 Info_size = sizeof(Info);
			RMint32 lastOKInstance;
			
			while (RUAGetBuffer(pDMA, &buf,  GETBUFFER_TIMEOUT_US) != RM_OK) {
				update_hdmi(&dcc_info, &disp_opt, &audio_opt);
				err = process_key(&dcc_info, &cmd, KEYFLAGS);
				if (cmd == RM_QUIT) goto cleanup;
			}
			switch (audio_gen_type) {
			case audio_gen_tri:
				CreateTriangle(buf, (1 << play_opt.dmapool_log2size), &count);
				break;
			case audio_gen_sin:
				CreateSine(buf, (1 << play_opt.dmapool_log2size), &count, audio_freq, audio_opt.SampleRate, FALSE, dB);
				break;
			case audio_gen_cos:
				CreateSine(buf, (1 << play_opt.dmapool_log2size), &count, audio_freq, audio_opt.SampleRate, TRUE, dB);
				break;
			default:
				count = 0;
			}
			while (DCCMultipleAudioSendData(dcc_info.pMultipleAudioSource, pDMA, buf, count, pInfo, Info_size, &lastOKInstance) != RM_OK) {
				struct RUAEvent e;
				struct DCCAudioSourceHandle audioHandle;
				
				status = DCCMultipleAudioSourceGetSingleDecoderHandleForInstance(dcc_info.pMultipleAudioSource, lastOKInstance, &audioHandle);
				e.ModuleID = audioHandle.moduleID;
				e.Mask = RUAEVENT_XFER_FIFO_READY;
				
				while (RUAWaitForMultipleEvents(dcc_info.pRUA, &e, 1, SENDDATA_TIMEOUT_US, NULL) != RM_OK) {
					fprintf(stderr, "wait for xfer fifo ready\n");
					update_hdmi(&dcc_info, &disp_opt, &audio_opt);
					err = process_key(&dcc_info, &cmd, KEYFLAGS);
					if (cmd == RM_QUIT) goto cleanup;
				}
			}
			RUAReleaseBuffer(pDMA, buf);
			buf = NULL;
			update_hdmi(&dcc_info, &disp_opt, &audio_opt);
		} else {
			update_hdmi(&dcc_info, &disp_opt, NULL);
		}
	} while(1);
	
cleanup:
	RMTermExit();
	
 	enable = FALSE;
	while ((err = RUASetProperty(pInstance, colorbars, RMGenericPropertyID_Enable, &enable, sizeof(enable), 0)) == RM_PENDING);
	if (err != RM_OK) {
		RMDBGLOG((ENABLE, "Cannot enable color bars, %d\n", err));
		return -1;
	}
	
	clear_display_options(&dcc_info, &disp_opt);
	
	if (audio_generate) {
		err = DCCStopMultipleAudioSource(dcc_info.pMultipleAudioSource);
		if (RMFAILED(err)) {
			fprintf(stderr, "Cannot stop audio decoder! %s\n", RMstatusToString(err));
		}
		
		err = DCCCloseMultipleAudioSource(dcc_info.pMultipleAudioSource);
		dcc_info.pMultipleAudioSource = NULL;
		if (RMFAILED(err)) {
			fprintf(stderr, "Error, cannot close audio decoder! %s\n", RMstatusToString(err));
		}
	}
	
	err = DCCClose(pDCC);
	if (RMFAILED(err)) {
		RMDBGLOG((ENABLE, "Cannot close DCC %d\n", err));
		return -1;
	}
	
	RUADestroyInstance(pInstance);
	
	return 0;
}
}

Ret ftk_backend_init(int argc, char* argv[])
{
	ftk_set_display(ftk_display_sigma_create());
	if(ftk_default_display() == NULL)
	{
		ftk_loge("open display failed.\n");
		exit(0);
	}

	return RET_OK;
}

