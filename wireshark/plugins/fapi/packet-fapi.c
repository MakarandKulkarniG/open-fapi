#include "config.h"

#include <epan/packet.h>

#define FAPI_PORT_RD 					58140U
#define FAPI_PORT_WR 					58142U

#define FAPI_HIDCI0_HIDCI0PDU_INFO_HIPDU_SIZE		(8)
#define FAPI_HIDCI0_HIDCI0PDU_INFO_DCI0PDU_SIZE		(24)
#define FAPI_RACH_IND_RACHPDU_INFO_SIZE			(8)
#define FAPI_ULHARQ_IND_HARQPDU_INFO_SIZE		(8)
#define FAPI_ULSCHHARQPDU_ULSCHPDU_INFO_SIZE		(20)
#define FAPI_ULSCHHARQPDU_HARQ_INFO_SIZE		(4)
#define FAPI_ULSCHHARQPDU_INITIALTXPARAM_INFO_SIZE	(4)
#define FAPI_SRHARQPDU_HARQ_INFO_SIZE						(8)
#define FAPI_SRHARQPDU_SR_INFO_SIZE						(2)

static int proto_fapi                                   							= -1;

static int hf_fapi_message_type                         							= -1;
static int hf_fapi_message_len                          							= -1;
static int hf_fapi_message_vendor_tlv_len               							= -1;
static int hf_fapi_message_body                         							= -1;

static int hf_fapi_subframe_ind                         							= -1;
static int hf_fapi_subframe_ind_sfnsf                   							= -1;
static int hf_fapi_subframe_ind_sfnsf_sfn               							= -1;
static int hf_fapi_subframe_ind_sfnsf_sf                							= -1;
static int hf_fapi_subframe_ind_padding                 							= -1;

static int hf_fapi_dlconfig_req                         							= -1;
static int hf_fapi_dlconfig_req_sfnsf                   							= -1;
static int hf_fapi_dlconfig_req_sfnsf_sfn               							= -1;
static int hf_fapi_dlconfig_req_sfnsf_sf                							= -1;
static int hf_fapi_dlconfig_req_len                     							= -1;
static int hf_fapi_dlconfig_req_cfi                     							= -1;
static int hf_fapi_dlconfig_req_numDCI                  							= -1;
static int hf_fapi_dlconfig_req_numOfPDU                							= -1;
static int hf_fapi_dlconfig_req_txPowerForPCFICH        							= -1;
static int hf_fapi_dlconfig_req_numOfPDSCHRNTI          							= -1;
static int hf_fapi_dlconfig_req_padding                 							= -1;
static int hf_fapi_dlconfig_req_pdu_info                							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pdutype								= -1;
static int hf_fapi_dlconfig_req_pdu_info_pdusize								= -1;
static int hf_fapi_dlconfig_req_pdu_info_pdupadding								= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion								= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dciformat						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_cceIndex						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_rnti							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlvl					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype				= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_mcs 						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rv						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_ndi						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_tpc						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_dai						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_txpower					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_padding					= -1;

static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlvl					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment				= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs 						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rv						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_dai						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_padding					= -1;

static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pdulen						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pduidx						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rnti							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_resallocationtype					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_vrbassignment					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rbcoding						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_mcs							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rv							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transportblocks					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_tb2cwswapflag					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transmissionscheme					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numlayers						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numsubbands						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_uecategory						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pa							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex				= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_ngap							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_nprb							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numrbpersubband					= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numbfVector						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_subbandInfo						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo				= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx			= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna			= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna		= -1;

static int hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pdulen							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pduidx							= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_txpower						= -1;
static int hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_padding						= -1;

static int hf_fapi_ulconfig_req											= -1;
static int hf_fapi_ulconfig_req_sfnsf										= -1;
static int hf_fapi_ulconfig_req_sfnsf_sfn									= -1;
static int hf_fapi_ulconfig_req_sfnsf_sf										= -1;
static int hf_fapi_ulconfig_req_ulconfiglen									= -1;
static int hf_fapi_ulconfig_req_numpdu										= -1;
static int hf_fapi_ulconfig_req_rachfreqresources								= -1;
static int hf_fapi_ulconfig_req_rachfreqresources_fdd								= -1;
static int hf_fapi_ulconfig_req_srs_present									= -1;
static int hf_fapi_ulconfig_req_padding										= -1;
static int hf_fapi_ulconfig_req_pdu_info									= -1;
static int hf_fapi_ulconfig_req_pdu_info_pdutype								= -1;
static int hf_fapi_ulconfig_req_pdu_info_pdusize								= -1;
static int hf_fapi_ulconfig_req_pdu_info_padding								= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo								= -1;

static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu							= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_handle						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_rnti						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_length						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_dataoffset					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_timingadvance					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ulcqi						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ri						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_padding						= -1;

static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu							= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_handle						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_rnti						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_padding						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_handle					        = -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_rnti						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu							= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_handle						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_rnti						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex					= -1;

static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu							= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_handle						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_size						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rnti						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rbstart						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_numofrb						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_modulationtype					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_ndi						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rv						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_harqproc					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_txmode						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_currtxnb					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_nsrs						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu						= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs		= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled		= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits		= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo					= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode			= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo				= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial		= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb		= -1;
static int hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding			= -1;

static int hf_fapi_dldatatx_req											= -1;
static int hf_fapi_dldatatx_req_sfnsf										= -1;
static int hf_fapi_dldatatx_req_sfnsf_sfn									= -1;
static int hf_fapi_dldatatx_req_sfnsf_sf									= -1;
static int hf_fapi_dldatatx_req_numofpdu									= -1;
static int hf_fapi_dldatatx_req_dlpdu_info									= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_pdulen								= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_pduidx								= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_numoftlv								= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_tlvinfo								= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_tag								= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen							= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_padding							= -1;
static int hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_value							= -1;

static int hf_fapi_rxulsch_ind											= -1;
static int hf_fapi_rxulsch_ind_sfnsf										= -1;
static int hf_fapi_rxulsch_ind_sfnsf_sfn									= -1;
static int hf_fapi_rxulsch_ind_sfnsf_sf										= -1;
static int hf_fapi_rxulsch_ind_numofpdu										= -1;
static int hf_fapi_rxulsch_ind_datapduinfo									= -1;
static int hf_fapi_rxulsch_ind_pdubuffer									= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_handle								= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_rnti									= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_length								= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_dataoffset								= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_timingadvance							= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_ulcqi								= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_padding								= -1;
static int hf_fapi_rxulsch_ind_datapduinfo_data									= -1;

static int hf_fapi_ulcrc_ind											= -1;
static int hf_fapi_ulcrc_ind_sfnsf										= -1;
static int hf_fapi_ulcrc_ind_sfnsf_sfn										= -1;
static int hf_fapi_ulcrc_ind_sfnsf_sf										= -1;
static int hf_fapi_ulcrc_ind_numofcrc										= -1;
static int hf_fapi_ulcrc_ind_crcpdu										= -1;
static int hf_fapi_ulcrc_ind_crcpdu_handle									= -1;
static int hf_fapi_ulcrc_ind_crcpdu_rnti									= -1;
static int hf_fapi_ulcrc_ind_crcpdu_crcflag									= -1;
static int hf_fapi_ulcrc_ind_crcpdu_padding									= -1;

static int hf_fapi_hidci0_ind											= -1;
static int hf_fapi_hidci0_sfnsf											= -1;
static int hf_fapi_hidci0_sfnsf_sfn										= -1;
static int hf_fapi_hidci0_sfnsf_sf										= -1;
static int hf_fapi_hidci0_numofdci										= -1;
static int hf_fapi_hidci0_numofhi										= -1;
static int hf_fapi_hidci0_hidci0pdu_info									= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu									= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_pdutype								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_hipdusize							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_rbstart								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs						= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_hivalue								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_iphich								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hipdu_txpower								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0									= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_pdutype								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_dciformat							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_cceindex							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_rnti								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_agglevel							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_rbstart								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_numofrb								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs						= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_mcs								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_newdataindication						= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection						= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_tpc								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_cqirequest							= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_ulindex								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex						= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_padding								= -1;
static int hf_fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap							= -1;

static int hf_fapi_rach_ind											= -1;
static int hf_fapi_rach_ind_sfnsf										= -1;
static int hf_fapi_rach_ind_sfnsf_sfn										= -1;
static int hf_fapi_rach_ind_sfnsf_sf										= -1;
static int hf_fapi_rach_ind_numofpreambles									= -1;
static int hf_fapi_rach_ind_rachpdu_info									= -1;
static int hf_fapi_rach_ind_rachpdu_info_rnti									= -1;
static int hf_fapi_rach_ind_rachpdu_info_timingadvance								= -1;
static int hf_fapi_rach_ind_rachpdu_info_preamble								= -1;
static int hf_fapi_rach_ind_rachpdu_info_padding								= -1;

static int hf_fapi_ulharq_ind											= -1;
static int hf_fapi_ulharq_ind_sfnsf										= -1;
static int hf_fapi_ulharq_ind_sfnsf_sfn										= -1;
static int hf_fapi_ulharq_ind_sfnsf_sf										= -1;
static int hf_fapi_ulharq_ind_numofharq										= -1;
static int hf_fapi_ulharq_ind_harqpdu_info									= -1;
static int hf_fapi_ulharq_ind_harqpdu_info_handle								= -1;
static int hf_fapi_ulharq_ind_harqpdu_info_rnti									= -1;
static int hf_fapi_ulharq_ind_harqpdu_info_harqtb1								= -1;
static int hf_fapi_ulharq_ind_harqpdu_info_harqtb2								= -1;

static gint ett_fapi_message_type                       							= -1;
static gint ett_fapi_message_len                        							= -1;
static gint ett_fapi_message_vendor_tlv_len             							= -1;
static gint ett_fapi_message_body                        							= -1;

static gint ett_fapi_subframe_ind                       							= -1;
static gint ett_fapi_subframe_ind_sfnsf                 							= -1;
static gint ett_fapi_subframe_ind_sfnsf_sfn             							= -1;
static gint ett_fapi_subframe_ind_sfnsf_sf              							= -1;
static gint ett_fapi_subframe_ind_padding               							= -1;

static gint ett_fapi_dlconfig_req                       							= -1;
static gint ett_fapi_dlconfig_req_sfnsf                       							= -1;
static gint ett_fapi_dlconfig_req_sfnsf_sfn                    							= -1;
static gint ett_fapi_dlconfig_req_sfnsf_sf                     							= -1;
static gint ett_fapi_dlconfig_req_len                   							= -1;
static gint ett_fapi_dlconfig_req_cfi                   							= -1;
static gint ett_fapi_dlconfig_req_numDCI                							= -1;
static gint ett_fapi_dlconfig_req_numOfPDU              							= -1;
static gint ett_fapi_dlconfig_req_txPowerForPCFICH      							= -1;
static gint ett_fapi_dlconfig_req_numOfPDSCHRNTI        							= -1;
static gint ett_fapi_dlconfig_req_padding               							= -1;
static gint ett_fapi_dlconfig_req_pdu_info              							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pdutype								= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pdusize								= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pdupadding								= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion								= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dciformat						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_cceIndex						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_rnti							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu						= -1;

static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlvl 					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype 				= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_mcs 					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rv						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_ndi						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_tpc						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_dai						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_txpower					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_padding					= -1;

static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlvl 					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment 				= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs 					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rv						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_dai					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx				= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx				= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_padding					= -1;

static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pdulen						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pduidx						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rnti						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_resallocationtype					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_vrbassignment					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rbcoding						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_mcs						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rv							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transportblocks					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_tb2cwswapflag					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transmissionscheme					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numlayers						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numsubbands					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_uecategory						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pa							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex				= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_ngap						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_nprb						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numrbpersubband					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numbfVector					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_subbandInfo					= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo				= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx			= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna			= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna		= -1;

static gint ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu							= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pdulen						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pduidx						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_txpower						= -1;
static gint ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_padding						= -1;

static gint ett_fapi_ulconfig_req										= -1;
static gint ett_fapi_ulconfig_req_sfnsf										= -1;
static gint ett_fapi_ulconfig_req_sfnsf_sfn									= -1;
static gint ett_fapi_ulconfig_req_sfnsf_sf									= -1;
static gint ett_fapi_ulconfig_req_ulconfiglen									= -1;
static gint ett_fapi_ulconfig_req_numpdu									= -1;
static gint ett_fapi_ulconfig_req_rachfreqresources								= -1;
static gint ett_fapi_ulconfig_req_rachfreqresources_fdd								= -1;
static gint ett_fapi_ulconfig_req_srs_present									= -1;
static gint ett_fapi_ulconfig_req_padding									= -1;
static gint ett_fapi_ulconfig_req_pdu_info									= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pdutype								= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pdusize								= -1;
static gint ett_fapi_ulconfig_req_pdu_info_padding								= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo							= -1;

static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu							= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_handle						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_rnti						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_length						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_dataoffset					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_timingadvance					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ulcqi						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ri						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_padding						= -1;

static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_handle						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_rnti						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_padding					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_handle					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_rnti						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu							= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_handle						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_rnti						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex				= -1;

static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_handle					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_size						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rnti						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rbstart					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_numofrb					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_modulationtype				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_ndi						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rv						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_harqproc					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_txmode					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_currtxnb					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_nsrs						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu						= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype		= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs		= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled		= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits		= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo					= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding				= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo			= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial		= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb		= -1;
static gint ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding		= -1;

static gint ett_fapi_dldatatx_req										= -1;
static gint ett_fapi_dldatatx_req_sfnsf										= -1;
static gint ett_fapi_dldatatx_req_sfnsf_sfn									= -1;
static gint ett_fapi_dldatatx_req_sfnsf_sf									= -1;
static gint ett_fapi_dldatatx_req_numofpdu									= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info									= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_pdulen								= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_pduidx								= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_numoftlv								= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_tlvinfo								= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_tag							= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen							= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_padding							= -1;
static gint ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_value							= -1;

static gint ett_fapi_rxulsch_ind										= -1;
static gint ett_fapi_rxulsch_ind_sfnsf										= -1;
static gint ett_fapi_rxulsch_ind_sfnsf_sfn									= -1;
static gint ett_fapi_rxulsch_ind_sfnsf_sf									= -1;
static gint ett_fapi_rxulsch_ind_numofpdu									= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo									= -1;
static gint ett_fapi_rxulsch_ind_pdubuffer									= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_handle								= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_rnti								= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_length								= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_dataoffset								= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_timingadvance							= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_ulcqi								= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_padding								= -1;
static gint ett_fapi_rxulsch_ind_datapduinfo_data								= -1;

static gint ett_fapi_ulcrc_ind											= -1;
static gint ett_fapi_ulcrc_ind_sfnsf										= -1;
static gint ett_fapi_ulcrc_ind_sfnsf_sfn									= -1;
static gint ett_fapi_ulcrc_ind_sfnsf_sf										= -1;
static gint ett_fapi_ulcrc_ind_numofcrc										= -1;
static gint ett_fapi_ulcrc_ind_crcpdu										= -1;
static gint ett_fapi_ulcrc_ind_crcpdu_handle									= -1;
static gint ett_fapi_ulcrc_ind_crcpdu_rnti									= -1;
static gint ett_fapi_ulcrc_ind_crcpdu_crcflag									= -1;
static gint ett_fapi_ulcrc_ind_crcpdu_padding									= -1;

static gint ett_fapi_hidci0_ind											= -1;
static gint ett_fapi_hidci0_sfnsf										= -1;
static gint ett_fapi_hidci0_sfnsf_sfn										= -1;
static gint ett_fapi_hidci0_sfnsf_sf										= -1;
static gint ett_fapi_hidci0_numofdci										= -1;
static gint ett_fapi_hidci0_numofhi										= -1;
static gint ett_fapi_hidci0_hidci0pdu_info									= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu								= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_pdutype							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_hipdusize							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_rbstart							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_hivalue							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_iphich								= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hipdu_txpower							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0								= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_pdutype							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_dciformat							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_cceindex							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_rnti								= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_agglevel							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_rbstart							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_numofrb							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_mcs								= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_newdataindication						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_tpc								= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_cqirequest							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_ulindex							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex						= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_padding							= -1;
static gint ett_fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap							= -1;

static gint ett_fapi_rach_ind											= -1;
static gint ett_fapi_rach_ind_sfnsf										= -1;
static gint ett_fapi_rach_ind_sfnsf_sfn										= -1;
static gint ett_fapi_rach_ind_sfnsf_sf										= -1;
static gint ett_fapi_rach_ind_numofpreambles									= -1;
static gint ett_fapi_rach_ind_rachpdu_info									= -1;
static gint ett_fapi_rach_ind_rachpdu_info_rnti									= -1;
static gint ett_fapi_rach_ind_rachpdu_info_timingadvance							= -1;
static gint ett_fapi_rach_ind_rachpdu_info_preamble								= -1;
static gint ett_fapi_rach_ind_rachpdu_info_padding								= -1;

static gint ett_fapi_ulharq_ind											= -1;
static gint ett_fapi_ulharq_ind_sfnsf										= -1;
static gint ett_fapi_ulharq_ind_sfnsf_sfn									= -1;
static gint ett_fapi_ulharq_ind_sfnsf_sf									= -1;
static gint ett_fapi_ulharq_ind_numofharq									= -1;
static gint ett_fapi_ulharq_ind_harqpdu_info									= -1;
static gint ett_fapi_ulharq_ind_harqpdu_info_handle								= -1;
static gint ett_fapi_ulharq_ind_harqpdu_info_rnti								= -1;
static gint ett_fapi_ulharq_ind_harqpdu_info_harqtb1								= -1;
static gint ett_fapi_ulharq_ind_harqpdu_info_harqtb2								= -1;

static const value_string message_id_vals[] = {
    { 0x00, "PARAM.request" },
    { 0x01, "PARAM.response" },
    { 0x02, "CELL_CONFIG.request" },
    { 0x03, "CELL_CONFIG.response" },
    { 0x04, "START.request" },
    { 0x05, "STOP.request" },
    { 0x06, "STOP.indication" },
    { 0x07, "UE_CONFIG.request" },
    { 0x08, "UE_CONFIG.response" },
    { 0x09, "ERROR.indication" },
  
    { 0x80, "DL_CONFIG.request" },
    { 0x81, "UL_CONFIG.request" },
    { 0x82, "UL_SUBFRAME.indication" },
    { 0x83, "DL_HI_DCI0.request" },
    { 0x84, "DL_TX.request" },
    { 0x85, "UL_HARQ.indication" },
    { 0x86, "UL_CRC.indication" },
    { 0x87, "UL_RX_ULSCH.indication" },
    { 0x88, "UL_RACH.indication" },
    { 0x89, "UL_RX_SR.indication" },
    { 0x8a, "UL_RX_CQI.indication" }
}; 

static const value_string dlconfig_req_pdutype_vals[] = {
	{ 0, "DCI PDU" },
	{ 1, "BCH PDU" },
	{ 2, "MCH PDU" },
	{ 3, "DLSCH PDU" },
	{ 4, "PCH PDU" },
};

static const value_string dlconfig_req_dciformat_vals[] = {
	{0, "1" },
	{1, "1A"},
	{2, "1B"},
	{3, "1C"},
	{4, "1D"},
	{5, "2" },
	{6, "2A"},
};

static const value_string dlconfig_req_dci1_resallocationtype_vals[] = {
	{0, "Type 0"},
	{1, "Type 1"},
	{2, "Type 2"},
};

static const value_string dlconfig_req_dci1a_vrbassignment_vals[] = {
	{0, "localized"},
	{1, "distributed"},
};

static const value_string dlconfig_req_dci1a_rntitype_vals[] = {
	{1, "C-RNTI"},
	{2, "RA-RNTI, P-RNTI or SI-RNTI"},
	{3, "SPS-RNTI"},
};

static const value_string dlconfig_req_dlsch_txscheme_vals[] = {
	{0, "Single Antenna Port 0"},
	{1, "Transmit Diversity"},
	{2, "Large Delay CDD"},
	{3, "Closed Loop Spatial Multiplexing"},
	{4, "Multi-User MIMO"},
	{5, "Closed Loop Rank 1 Precoding"},
	{6, "Single Antenna Port 5"},
};

static const value_string ulconfig_req_ulsch_txmode_vals[] = {
	{0, "SISO/MIMO"},
	{1, "MIMO"},
};

static const value_string ulconfig_req_harqinfo_acknackmode_vals[] = {
	{0, "BUNDLING"},
	{1, "MULTIPLEXING"},
};
static const value_string dlconfig_req_dlsch_pa_vals[] = {
	{0, "-6 dB"},
	{1, "-4.77 dB"},
	{2, "-3 dB"},
	{3, "-1.77 dB"},
	{4, "0 dB"},
	{5, "1 dB"},
	{6, "2 dB"},
	{7, "3 dB"},
};

static const value_string ulconfig_req_pdutype_vals[] = {
	{0, "ULSCH"},
	{1, "ULSCH CQI RI"},
	{2, "ULSCH HARQ"},
	{3, "ULSCH CQI HARQ RI"},
	{4, "UCI CQI"},
	{5, "UCI SR"},
	{6, "UCI HARQ"},
	{7, "UCI SR HARQ"},
	{8, "UCI CQI HARQ"},
	{9, "UCI CQI SR"},
	{10, "UCI CQI SR HARQ"},
	{11, "SRS"},
	{12, "HARQ BUFFER"},
};

static const value_string ulcrc_ind_crcflag_vals[] = {
	{0, "PASS"},
	{1, "FAIL"},
};

static const value_string hidci0_ind_hivalue_vals[] = {
	{0, "NACK"},
	{1, "ACK"},
};

static const value_string hidci0_ind_dciformat_vals[] = {
	{0, "0"},
	{1, "3"},
	{2, "3A"},
};

static const value_string hidci0_ind_uetxantennaselection_vals[] = {
	{0, "Not configured"},
	{1, "Configured and using UE port 0"},
	{2, "Configured and using UE port 1"},
};

static const value_string hidci0_ind_cqirequest_vals[] = {
	{0, "Aperiodic cQI not requested"},
	{1, "Aperiodic CQI requested"},
};

static const value_string ulharq_ind_harqpdu_info_harq_vals[] = {
	{1, "ACK"},
	{2, "NACK"},
	{3, "ACK or NACK"},
	{4, "DTX"},
	{5, "ACK or DTX"},
	{6, "NACK or DTX"},
	{7, "ACK or NACK or DTX"},
};

static int dissect_fapi_subframe_sfnsf(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_)
{
        proto_item *fapi_subframe_ind_sfnsf_item = proto_tree_add_item(tree, hf_fapi_subframe_ind_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_subframe_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_subframe_ind_sfnsf_item, ett_fapi_subframe_ind_sfnsf_sfn);
        proto_tree_add_item(fapi_subframe_ind_sfnsf_sfn_tree, hf_fapi_subframe_ind_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_subframe_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_subframe_ind_sfnsf_item, ett_fapi_subframe_ind_sfnsf_sf);
        proto_tree_add_item(fapi_subframe_ind_sfnsf_sf_tree, hf_fapi_subframe_ind_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlevel_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlvl);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlevel_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlvl, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_mcs_1_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_mcs);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_mcs_1_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_mcs, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_redundancyversion_1_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rv);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_redundancyversion_1_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rv, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_ndi_1_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_ndi);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_ndi_1_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_ndi, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_tpc_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_tpc);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_tpc_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_tpc, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_dai_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_dai);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_dai_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_dai, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_txpower_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_txpower);

    proto_item *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_txpower_item =
	    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_txpower_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_txpower, tvb, *offset, 2, ENC_BIG_ENDIAN);
    gint16 txPower = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);
    proto_item_append_text(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_txpower_item, " (%g dBm)", (float)(txPower - 6000)/1000);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_padding_tree =
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_padding);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_padding_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_padding, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_) 
{
    proto_item *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlevel_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlvl);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlevel_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlvl, tvb, *offset, 1, ENC_NA);

    *offset += 1;
    
    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment, tvb, *offset, 1, ENC_NA);

    *offset += 1;
     
    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs_1_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs_1_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_redundancyversion_1_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rv);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_redundancyversion_1_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rv, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi_1_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi_1_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_dai_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_dai);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_dai_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_dai, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower);

    proto_item *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower_item =
	    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower, tvb, *offset, 2, ENC_BIG_ENDIAN);
    gint16 txPower = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);
    proto_item_append_text(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower_item, " (%g dBm)", (float)(txPower - 6000)/1000);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_padding_tree = 
	    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_padding);

    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_padding_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_padding, tvb, *offset, 2, ENC_NA);

    *offset += 2;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset)
{
    guint8 pdu_size = 2 * (tvb_get_guint8(tvb, *offset) + 1);

    proto_item *fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_item 
	    = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_item,
		    ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx,
		    tvb, *offset, 1, ENC_NA);
    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_item,
		    ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna,
		    tvb, *offset, 1, ENC_NA);

    guint8 numAntenna = tvb_get_guint8(tvb, *offset);
    *offset += 1;

    if (numAntenna) {
        proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna_item = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_item,
		    ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna);
        proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna_item, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna,
		    tvb, *offset, 2*numAntenna, ENC_NA);

	*offset += (2*numAntenna); // 16 bits per antenna
    }
    return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
	proto_item *fapi_dlconfig_pdu_info_pduunion_dlschpdu_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu, tvb, *offset, pdu_size, ENC_NA);

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_pdulen_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pdulen);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_pdulen_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pdulen, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_pduidx_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pduidx);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_pduidx_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pduidx, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_rnti_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rnti);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_rnti_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_resallocationtype_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_resallocationtype);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_resallocationtype_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_resallocationtype, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_vrbassignment_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_vrbassignment);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_vrbassignment_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_vrbassignment, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_rbcoding_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rbcoding);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_rbcoding_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rbcoding, tvb, *offset, 4, ENC_BIG_ENDIAN);

	*offset += 4;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_mcs_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_mcs);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_mcs_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_mcs, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_rv_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rv);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_rv_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rv, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_transportblocks_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transportblocks);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_transportblocks_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transportblocks, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_tb2cwswapflag_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_tb2cwswapflag);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_tb2cwswapflag_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_tb2cwswapflag, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_transmissionscheme_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transmissionscheme);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_transmissionscheme_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transmissionscheme, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_numlayers_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numlayers);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_numlayers_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numlayers, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_numsubbands_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numsubbands);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_numsubbands_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numsubbands, tvb, *offset, 1, ENC_NA);

	guint8 numSubBands = tvb_get_guint8(tvb, *offset);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_uecategory_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_uecategory);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_uecategory_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_uecategory, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_pa_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pa);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_pa_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pa, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_ngap_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_ngap);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_ngap_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_ngap, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_nprb_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_nprb);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_nprb_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_nprb, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_numrbpersubband_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numrbpersubband);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_numrbpersubband_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numrbpersubband, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_numbfvectors_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numbfVector);
	proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_numbfvectors_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numbfVector, tvb, *offset, 2, ENC_BIG_ENDIAN);

	guint16 numBfVectors = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 2;

	if (numSubBands) {

	   proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_subbandinfo_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_subbandInfo);
	   proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dlschpdu_subbandinfo_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_subbandInfo, tvb, *offset, numSubBands, ENC_BIG_ENDIAN);

	   *offset += numSubBands;
        }

	if (numBfVectors) {
	   guint16 i;
	   for (i = 0; i < numBfVectors; i++) {
                proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_bfvectors_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dlschpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo);
		dissect_fapi_dlconfig_pdu_info_pduunion_dlschpdu_beamformingvectorinfo(tvb, pinfo, fapi_dlconfig_pdu_info_pduunion_dlschpdu_bfvectors_tree, data, offset);
	   }
	}
	return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_req_pdu_info_pduunion_bchpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_dlconfig_pdu_info_pduunion_bchpdu_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_dlconfig_pdu_info_pduunion_bchpdu_pdulen_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_bchpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pdulen);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_bchpdu_pdulen_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pdulen, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_bchpdu_pduidx_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_bchpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pduidx);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_bchpdu_pduidx_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pduidx, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_bchpdu_txpower_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_bchpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_txpower);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_bchpdu_txpower_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_txpower, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_bchpdu_padding_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_bchpdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_padding);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_bchpdu_padding_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_padding, tvb, *offset, 2, ENC_NA);

    *offset += 2;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_req_pdu_info_pduunion_dcipdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    guint8 dciFormat = tvb_get_guint8(tvb, *offset);

    proto_item *fapi_dlconfig_pdu_info_pduunion_dcipdu_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dciformat_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dciformat);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dciformat_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dciformat, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_cceindex_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_cceIndex);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_cceindex_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_cceIndex, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_rnti_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_rnti);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_rnti_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu);
    proto_item *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_item = 
	    proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_tree, hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu, tvb, *offset, pdu_size - 4, ENC_NA);

    switch (dciFormat) {
	case 0: {
	    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_tree =
		    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1);
	    dissect_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1(tvb, pinfo, fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1_tree, data, offset, pdu_size - 4);
	}
	case 1: {
	    proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_tree = 
		    proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a);
	    dissect_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a(tvb, pinfo, fapi_dlconfig_pdu_info_pduunion_dcipdu_dcipdu_1a_tree, data, offset, pdu_size - 4);
	}
	break;
    }

    return tvb_captured_length(tvb);
}


static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_srinfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo, tvb, *offset, -1, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_srinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo, tvb, *offset, pdu_size, ENC_NA);
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);
    *offset += 4;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size, tvb, *offset, 2, ENC_BIG_ENDIAN);
    *offset += 2;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);
    *offset += 2;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo, tvb, *offset, pdu_size, ENC_NA);
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding, tvb, *offset, 2, ENC_NA);

    *offset += 2;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
	proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu, tvb, *offset, pdu_size, ENC_NA);

    	proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo);
	dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_tree, data, offset, FAPI_ULSCHHARQPDU_ULSCHPDU_INFO_SIZE);

    	proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo);
	dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_tree, data, offset, FAPI_ULSCHHARQPDU_HARQ_INFO_SIZE);

    	proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparam_info_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo);
	dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparam_info_tree, data, offset, FAPI_ULSCHHARQPDU_INITIALTXPARAM_INFO_SIZE);

	return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_handle_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_handle);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_handle_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_size_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_size);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_size_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_size, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_rnti_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rnti);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_rnti_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_rbstart_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rbstart);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_rbstart_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rbstart, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_numofrb_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_numofrb);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_numofrb_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_numofrb, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_modulationtype_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_modulationtype);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_modulationtype_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_modulationtype, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_ndi_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_ndi);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_ndi_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_ndi, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_rv_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rv);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_rv_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rv, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_harqproc_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_harqproc);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_harqproc_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_harqproc, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_txmode_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_txmode);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_txmode_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_txmode, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_currtxnb_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_currtxnb);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_currtxnb_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_currtxnb, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_nsrs_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_nsrs);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_nsrs_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_nsrs, tvb, *offset, 1, ENC_NA);

    *offset += 1;
    return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_handle_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_handle);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_handle_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_rnti_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_rnti);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_rnti_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_srinfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo);
    dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_srinfo_tree, data, offset, pdu_size - 6);

    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1, tvb, *offset, 2, ENC_BIG_ENDIAN);
    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding, tvb, *offset, 3, ENC_NA);

    *offset += 3;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo, tvb, *offset, pdu_size, ENC_NA);
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex, tvb, *offset, 2, ENC_BIG_ENDIAN);
    *offset += 2;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1, tvb, *offset, 2, ENC_BIG_ENDIAN);
    *offset += 2;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize, tvb, *offset, 1, ENC_NA);
    *offset += 1;
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding, tvb, *offset, 3, ENC_NA);
    *offset += 3;
    return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_srinfo_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo, tvb, *offset, -1, ENC_NA);
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_srinfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex, tvb, *offset, 2, ENC_BIG_ENDIAN);
    *offset += 2;
    return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
        proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu, tvb, *offset, pdu_size, ENC_NA);
        proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_handle_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_handle);
        proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_handle_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);
        *offset += 4;
        proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_rnti_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_rnti);
        proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_rnti_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);
	*offset += 2;
    	proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo);
	dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_harqinfo_tree, data, offset, FAPI_SRHARQPDU_HARQ_INFO_SIZE);
    	proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_srinfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo);
	dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_srinfo_tree, data, offset, FAPI_SRHARQPDU_SR_INFO_SIZE);
	return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_handle_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_handle);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_handle_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_rnti_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_rnti);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_rnti_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_padding_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_padding, tvb, *offset, 2, ENC_NA);

    *offset += 2;
    
    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo);
    dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_harqinfo_tree, data, offset, pdu_size);

    return tvb_captured_length(tvb);
}
static int dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint8 pdu_size _U_)
{
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_handle_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_handle);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_handle_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_rnti_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_rnti);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_rnti_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_length_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_length);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_length_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_length, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_dataoffset_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_dataoffset);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_dataoffset_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_dataoffset, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_timingadvance_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_timingadvance);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_timingadvance_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_timingadvance, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_ulcqi_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ulcqi);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_ulcqi_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ulcqi, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_ri_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ri);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_ri_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ri, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_padding_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_padding, tvb, *offset, 2, ENC_NA);

    *offset += 2;
    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_pdu_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_)
{
    guint8 pdu_type = 0;
    guint8 pdu_size = 0;

    pdu_size = tvb_get_guint8(tvb, *offset + 1);

    proto_item *fapi_ulconfig_pdu_info_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req_pdu_info, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulconfig_pdu_info_pdu_type_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_item, ett_fapi_ulconfig_req_pdu_info_pdutype);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pdu_type_tree, hf_fapi_ulconfig_req_pdu_info_pdutype, tvb, *offset, 1, ENC_NA);

    pdu_type = tvb_get_guint8(tvb, *offset);

    proto_item_append_text(fapi_ulconfig_pdu_info_item, " (%s) ", val_to_str_const(pdu_type, ulconfig_req_pdutype_vals, "Unknown (0x%02x)"));

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pdu_size_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_item, ett_fapi_ulconfig_req_pdu_info_pdusize);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pdu_size_tree, hf_fapi_ulconfig_req_pdu_info_pdusize, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulconfig_pdu_info_pdu_padding_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_item, ett_fapi_ulconfig_req_pdu_info_padding);
    proto_tree_add_item(fapi_ulconfig_pdu_info_pdu_padding_tree, hf_fapi_ulconfig_req_pdu_info_padding, tvb, *offset, 2, ENC_NA);

    *offset += 2;

    proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo);
    proto_item *fapi_ulconfig_pdu_info_pduconfiginfo_item = proto_tree_add_item(fapi_ulconfig_pdu_info_pduconfiginfo_tree, hf_fapi_ulconfig_req_pdu_info_pduconfiginfo, tvb, *offset, pdu_size - 2, ENC_NA);

    switch (pdu_type) {
	    case 0: { //ULSCH
                proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu);
	        dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_ulschpdu_tree, data, offset, pdu_size - 2);
	    }
            break;
            case 1: { //ULSCH CQI RI
	    }
            break;
            case 2: { //ULSCH HARQ
                proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu);
	        dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_ulschharqpdu_tree, data, offset, pdu_size - 2);
	    }
            break;
            case 3: { //ULSCH CQI HARQ RI
	    }
            break;
	    case 4: { //UCI CQI
                proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu);
	        dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_cqipdu_tree, data, offset, pdu_size);
            }
            break;
	    case 5: { //UCI SR
                proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu);
	        dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_srpdu_tree, data, offset, pdu_size - 2);
	    }
            break;
	    case 6: { //UCI HARQ
                proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu);
	        dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_harqpdu_tree, data, offset, pdu_size);
	    }
            break;
            case 7: { //UCI SR HARQ
                proto_tree *fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_tree = proto_item_add_subtree(fapi_ulconfig_pdu_info_pduconfiginfo_item, ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu);
	        dissect_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu(tvb, pinfo, fapi_ulconfig_pdu_info_pduconfiginfo_srharqpdu_tree, data, offset, pdu_size);
            }
            break;
    }
    return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_pdu_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_)
{
    guint8 pdu_type = 0;
    guint8 pdu_size = 0;

    pdu_size = tvb_get_guint8(tvb, *offset + 1);

    proto_item *fapi_dlconfig_pdu_info_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req_pdu_info, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_dlconfig_pdu_info_pdu_type_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_item, ett_fapi_dlconfig_req_pdu_info_pdutype);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pdu_type_tree, hf_fapi_dlconfig_req_pdu_info_pdutype, tvb, *offset, 1, ENC_NA);

    pdu_type = tvb_get_guint8(tvb, *offset);

    proto_item_append_text(fapi_dlconfig_pdu_info_item, " (%s) ", val_to_str_const(pdu_type, dlconfig_req_pdutype_vals, "Unknown (0x%02x)"));

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pdu_size_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_item, ett_fapi_dlconfig_req_pdu_info_pdusize);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pdu_size_tree, hf_fapi_dlconfig_req_pdu_info_pdusize, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_dlconfig_pdu_info_pdu_padding_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_item, ett_fapi_dlconfig_req_pdu_info_pdupadding);
    proto_tree_add_item(fapi_dlconfig_pdu_info_pdu_padding_tree, hf_fapi_dlconfig_req_pdu_info_pdupadding, tvb, *offset, 2, ENC_NA);

    *offset += 2;

    proto_tree *fapi_dlconfig_pdu_info_pduunion_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_item, ett_fapi_dlconfig_req_pdu_info_pduunion);
    proto_item *fapi_dlconfig_pdu_info_pduunion_item = proto_tree_add_item(fapi_dlconfig_pdu_info_pduunion_tree, hf_fapi_dlconfig_req_pdu_info_pduunion, tvb, *offset, pdu_size - 4, ENC_NA);

    switch (pdu_type) {
	    case 0: {
                proto_tree *fapi_dlconfig_pdu_info_pduunion_dcipdu_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu);
	        dissect_fapi_dlconfig_req_pdu_info_pduunion_dcipdu(tvb, pinfo, fapi_dlconfig_pdu_info_pduunion_dcipdu_tree, data, offset, pdu_size - 4);
	    }
	    break;
	    case 1: {
                proto_tree *fapi_dlconfig_pdu_info_pduunion_bchpdu_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_item, ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu);
	        dissect_fapi_dlconfig_req_pdu_info_pduunion_bchpdu(tvb, pinfo, fapi_dlconfig_pdu_info_pduunion_bchpdu_tree, data, offset, pdu_size - 4);
	    }
            break;
	    case 3: {
                proto_tree *fapi_dlconfig_pdu_info_pduunion_dlschpdu_tree = proto_item_add_subtree(fapi_dlconfig_pdu_info_pduunion_item, ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu);
	        dissect_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu(tvb, pinfo, fapi_dlconfig_pdu_info_pduunion_dlschpdu_tree, data, offset, pdu_size - 4);
	    }
	    default:
	    break;
    } /* switch (pdu_type) */

    return tvb_captured_length(tvb);
}
 
static int dissect_fapi_dldatatx_req_dlpdu_info_tlvinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 tag;
	guint16 taglen;

        proto_item *fapi_dldatatx_req_dlpdu_info_tlvinfo_item = proto_tree_add_item(tree, hf_fapi_dldatatx_req_dlpdu_info_tlvinfo, tvb, *offset, pdu_size, ENC_NA);

        proto_tree *fapi_dldatatx_req_dlpdu_info_tlvinfo_tag_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_tlvinfo_item, ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_tag);
        proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_tlvinfo_tag_tree, hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_tag, tvb, *offset, 2, ENC_BIG_ENDIAN);

        tag = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_tlvinfo_item, ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen);
        proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen_tree, hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen, tvb, *offset, 2, ENC_BIG_ENDIAN);

        taglen = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_dldatatx_req_dlpdu_info_tlvinfo_padding_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_tlvinfo_item, ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_padding);
        proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_tlvinfo_padding_tree, hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_padding, tvb, *offset, 4, ENC_BIG_ENDIAN);

	*offset += 4;

	if (tag == 0) {
            proto_tree *fapi_dldatatx_req_dlpdu_info_tlvinfo_value_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_tlvinfo_item, ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_value);
            proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_tlvinfo_value_tree, hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_value, tvb, *offset, taglen, ENC_NA);

	    *offset += (((taglen + 7) >> 3) * 8);
	}

	return tvb_captured_length(tvb);
}
static int dissect_fapi_dldatatx_req_dlpdu_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint numOfTlv;
	guint i;
	guint16 pduSize = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

        proto_item *fapi_dldatatx_req_dlpdu_info_item = proto_tree_add_item(tree, hf_fapi_dldatatx_req_dlpdu_info, tvb, *offset, pduSize, ENC_NA);

        proto_tree *fapi_dldatatx_req_dlpdu_info_pdulen_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_item, ett_fapi_dldatatx_req_dlpdu_info_pdulen);
        proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_pdulen_tree, hf_fapi_dldatatx_req_dlpdu_info_pdulen, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_dldatatx_req_dlpdu_info_pduidx_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_item, ett_fapi_dldatatx_req_dlpdu_info_pduidx);
        proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_pduidx_tree, hf_fapi_dldatatx_req_dlpdu_info_pduidx, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_dldatatx_req_dlpdu_info_numoftlv_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_item, ett_fapi_dldatatx_req_dlpdu_info_numoftlv);
        proto_tree_add_item(fapi_dldatatx_req_dlpdu_info_numoftlv_tree, hf_fapi_dldatatx_req_dlpdu_info_numoftlv, tvb, *offset, 4, ENC_BIG_ENDIAN);

	numOfTlv = tvb_get_guint32(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 4;

	for (i = 0; i < numOfTlv; i++) {
            proto_tree *fapi_dldatatx_req_dlpdu_info_tlvinfo_tree = proto_item_add_subtree(fapi_dldatatx_req_dlpdu_info_item, ett_fapi_dldatatx_req_dlpdu_info_tlvinfo);
            dissect_fapi_dldatatx_req_dlpdu_info_tlvinfo(tvb, pinfo, fapi_dldatatx_req_dlpdu_info_tlvinfo_tree, data, offset, pduSize - 8);
	}
         
	return tvb_captured_length(tvb);
}

static int dissect_fapi_rxulsch_ind_datapduinfo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_, guint *data_offset) 
{
	guint16 data_size = 0;

        proto_item *fapi_rxulsch_ind_datapduinfo_item = proto_tree_add_item(tree, hf_fapi_rxulsch_ind_datapduinfo, tvb, *offset, pdu_size, ENC_NA);

        proto_tree *fapi_rxulsch_ind_datapduinfo_handle_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_handle);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_handle_tree, hf_fapi_rxulsch_ind_datapduinfo_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

	*offset += 4;

        proto_tree *fapi_rxulsch_ind_datapduinfo_rnti_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_rnti);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_rnti_tree, hf_fapi_rxulsch_ind_datapduinfo_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_rxulsch_ind_datapduinfo_length_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_length);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_length_tree, hf_fapi_rxulsch_ind_datapduinfo_length, tvb, *offset, 2, ENC_BIG_ENDIAN);

	data_size = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_rxulsch_ind_datapduinfo_dataoffset_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_dataoffset);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_dataoffset_tree, hf_fapi_rxulsch_ind_datapduinfo_dataoffset, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_rxulsch_ind_datapduinfo_timingadvance_tree = 
		proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_timingadvance);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_timingadvance_tree, hf_fapi_rxulsch_ind_datapduinfo_timingadvance, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

        proto_tree *fapi_rxulsch_ind_datapduinfo_ulcqi_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_ulcqi);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_ulcqi_tree, hf_fapi_rxulsch_ind_datapduinfo_ulcqi, tvb, *offset, 1, ENC_NA);

	*offset += 1;

        proto_tree *fapi_rxulsch_ind_datapduinfo_padding_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_padding);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_padding_tree, hf_fapi_rxulsch_ind_datapduinfo_padding, tvb, *offset, 3, ENC_NA);

	*offset += 3;

        proto_tree *fapi_rxulsch_ind_datapduinfo_data_tree = proto_item_add_subtree(fapi_rxulsch_ind_datapduinfo_item, ett_fapi_rxulsch_ind_datapduinfo_data);
        proto_tree_add_item(fapi_rxulsch_ind_datapduinfo_data_tree, hf_fapi_rxulsch_ind_datapduinfo_data, tvb, *data_offset, data_size, ENC_NA);

	*data_offset += data_size;

	return tvb_captured_length(tvb);
}

static int dissect_fapi_ulharq_ind_harqpdu_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	proto_item *fapi_ulharq_ind_harqpdu_info_item = proto_tree_add_item(tree, hf_fapi_ulharq_ind_harqpdu_info, tvb, *offset, FAPI_ULHARQ_IND_HARQPDU_INFO_SIZE, ENC_NA);

	proto_tree *fapi_ulharq_ind_harqpdu_info_handle_tree = proto_item_add_subtree(fapi_ulharq_ind_harqpdu_info_item, ett_fapi_ulharq_ind_harqpdu_info_handle);
	proto_tree_add_item(fapi_ulharq_ind_harqpdu_info_handle_tree, hf_fapi_ulharq_ind_harqpdu_info_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

	*offset += 4;

	proto_tree *fapi_ulharq_ind_harqpdu_info_rnti_tree = proto_item_add_subtree(fapi_ulharq_ind_harqpdu_info_item, ett_fapi_ulharq_ind_harqpdu_info_rnti);
	proto_tree_add_item(fapi_ulharq_ind_harqpdu_info_rnti_tree, hf_fapi_ulharq_ind_harqpdu_info_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_ulharq_ind_harqpdu_info_harqtb1_tree = proto_item_add_subtree(fapi_ulharq_ind_harqpdu_info_item, ett_fapi_ulharq_ind_harqpdu_info_harqtb1);
	proto_tree_add_item(fapi_ulharq_ind_harqpdu_info_harqtb1_tree, hf_fapi_ulharq_ind_harqpdu_info_harqtb1, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_ulharq_ind_harqpdu_info_harqtb2_tree = proto_item_add_subtree(fapi_ulharq_ind_harqpdu_info_item, ett_fapi_ulharq_ind_harqpdu_info_harqtb2);
	proto_tree_add_item(fapi_ulharq_ind_harqpdu_info_harqtb2_tree, hf_fapi_ulharq_ind_harqpdu_info_harqtb2, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	return tvb_captured_length(tvb);
}

static int dissect_fapi_ulharq_ind(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 numOfHarq;

	proto_item *fapi_ulharq_ind_item = proto_tree_add_item(tree, hf_fapi_ulharq_ind, tvb, *offset, pdu_size, ENC_NA);

	proto_tree *fapi_ulharq_ind_sfnsf_tree = proto_item_add_subtree(fapi_ulharq_ind_item, ett_fapi_ulharq_ind_sfnsf);

        proto_item *fapi_ulharq_ind_sfnsf_item = proto_tree_add_item(fapi_ulharq_ind_sfnsf_tree, hf_fapi_ulharq_ind_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_ulharq_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_ulharq_ind_sfnsf_item, ett_fapi_ulharq_ind_sfnsf_sfn);
        proto_tree_add_item(fapi_ulharq_ind_sfnsf_sfn_tree, hf_fapi_ulharq_ind_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_ulharq_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_ulharq_ind_sfnsf_item, ett_fapi_ulharq_ind_sfnsf_sf);
        proto_tree_add_item(fapi_ulharq_ind_sfnsf_sf_tree, hf_fapi_ulharq_ind_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

	proto_tree *fapi_ulharq_ind_numofpreambles_tree = proto_item_add_subtree(fapi_ulharq_ind_item, ett_fapi_ulharq_ind_numofharq);
	proto_tree_add_item(fapi_ulharq_ind_numofpreambles_tree, hf_fapi_ulharq_ind_numofharq, tvb, *offset, 2, ENC_BIG_ENDIAN);

	numOfHarq = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 2;

	guint16 i;
	for (i = 0; i < numOfHarq; i++) {
		proto_tree *fapi_ulharq_ind_harqpdu_info_tree = proto_item_add_subtree(fapi_ulharq_ind_item, ett_fapi_ulharq_ind_harqpdu_info);
		dissect_fapi_ulharq_ind_harqpdu_info(tvb, pinfo, fapi_ulharq_ind_harqpdu_info_tree, data, offset, pdu_size - 4);
	}
	return tvb_captured_length(tvb);
}

static int dissect_fapi_rach_ind_rachpdu_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	proto_item *fapi_rach_ind_rachpdu_info_item = proto_tree_add_item(tree, hf_fapi_rach_ind_rachpdu_info, tvb, *offset, FAPI_RACH_IND_RACHPDU_INFO_SIZE, ENC_NA);

	proto_tree *fapi_rach_ind_rachpdu_info_rnti_tree = proto_item_add_subtree(fapi_rach_ind_rachpdu_info_item, ett_fapi_rach_ind_rachpdu_info_rnti);
	proto_tree_add_item(fapi_rach_ind_rachpdu_info_rnti_tree, hf_fapi_rach_ind_rachpdu_info_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_rach_ind_rachpdu_info_timingadvance_tree = proto_item_add_subtree(fapi_rach_ind_rachpdu_info_item, ett_fapi_rach_ind_rachpdu_info_timingadvance);
	proto_tree_add_item(fapi_rach_ind_rachpdu_info_timingadvance_tree, hf_fapi_rach_ind_rachpdu_info_timingadvance, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree *fapi_rach_ind_rachpdu_info_preamble_tree = proto_item_add_subtree(fapi_rach_ind_rachpdu_info_item, ett_fapi_rach_ind_rachpdu_info_preamble);
	proto_tree_add_item(fapi_rach_ind_rachpdu_info_preamble_tree, hf_fapi_rach_ind_rachpdu_info_preamble, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree *fapi_rach_ind_rachpdu_info_padding_tree = proto_item_add_subtree(fapi_rach_ind_rachpdu_info_item, ett_fapi_rach_ind_rachpdu_info_padding);
	proto_tree_add_item(fapi_rach_ind_rachpdu_info_padding_tree, hf_fapi_rach_ind_rachpdu_info_padding, tvb, *offset, 3, ENC_NA);

	*offset += 3;

	return tvb_captured_length(tvb);
}

static int dissect_fapi_rach_ind(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 numOfPreambles;

	proto_item *fapi_rach_ind_item = proto_tree_add_item(tree, hf_fapi_rach_ind, tvb, *offset, pdu_size, ENC_NA);

	proto_tree *fapi_rach_ind_sfnsf_tree = proto_item_add_subtree(fapi_rach_ind_item, ett_fapi_rach_ind_sfnsf);

        proto_item *fapi_rach_ind_sfnsf_item = proto_tree_add_item(fapi_rach_ind_sfnsf_tree, hf_fapi_rach_ind_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_rach_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_rach_ind_sfnsf_item, ett_fapi_rach_ind_sfnsf_sfn);
        proto_tree_add_item(fapi_rach_ind_sfnsf_sfn_tree, hf_fapi_rach_ind_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_rach_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_rach_ind_sfnsf_item, ett_fapi_rach_ind_sfnsf_sf);
        proto_tree_add_item(fapi_rach_ind_sfnsf_sf_tree, hf_fapi_rach_ind_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

	proto_tree *fapi_rach_ind_numofpreambles_tree = proto_item_add_subtree(fapi_rach_ind_item, ett_fapi_rach_ind_numofpreambles);
	proto_tree_add_item(fapi_rach_ind_numofpreambles_tree, hf_fapi_rach_ind_numofpreambles, tvb, *offset, 2, ENC_BIG_ENDIAN);

	numOfPreambles = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

	*offset += 2;

	guint16 i;
	for (i = 0; i < numOfPreambles; i++) {
		proto_tree *fapi_rach_ind_rachpdu_info_tree = proto_item_add_subtree(fapi_rach_ind_item, ett_fapi_rach_ind_rachpdu_info);
		dissect_fapi_rach_ind_rachpdu_info(tvb, pinfo, fapi_rach_ind_rachpdu_info_tree, data, offset, pdu_size - 4);
	}
	return tvb_captured_length(tvb);
}

static int dissect_fapi_rxulsch_ind(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 numOfPdu;
	guint16 i;
	gint 	data_offset;

        proto_item *fapi_rxulsch_ind_item = proto_tree_add_item(tree, hf_fapi_rxulsch_ind, tvb, *offset, pdu_size, ENC_NA);

        proto_tree *fapi_rxulsch_ind_sfnsf_tree = proto_item_add_subtree(fapi_rxulsch_ind_item, ett_fapi_subframe_ind_sfnsf);

        proto_item *fapi_rxulsch_ind_sfnsf_item = proto_tree_add_item(fapi_rxulsch_ind_sfnsf_tree, hf_fapi_rxulsch_ind_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_rxulsch_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_rxulsch_ind_sfnsf_item, ett_fapi_rxulsch_ind_sfnsf_sfn);
        proto_tree_add_item(fapi_rxulsch_ind_sfnsf_sfn_tree, hf_fapi_rxulsch_ind_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_rxulsch_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_rxulsch_ind_sfnsf_item, ett_fapi_rxulsch_ind_sfnsf_sf);
        proto_tree_add_item(fapi_rxulsch_ind_sfnsf_sf_tree, hf_fapi_rxulsch_ind_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_tree *fapi_rxulsch_ind_numofpdu_tree = proto_item_add_subtree(fapi_rxulsch_ind_item, ett_fapi_rxulsch_ind_numofpdu);
        proto_tree_add_item(fapi_rxulsch_ind_numofpdu_tree, hf_fapi_rxulsch_ind_numofpdu, tvb, *offset, 2, ENC_BIG_ENDIAN);

	numOfPdu = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

        *offset += 2;

	for (i = 0; i < numOfPdu; i++) {
	    data_offset = *offset + 16 * numOfPdu;// offset where data starts
            proto_tree *fapi_rxulsch_ind_datapduinfo_tree = proto_item_add_subtree(fapi_rxulsch_ind_item, ett_fapi_rxulsch_ind_datapduinfo);
            dissect_fapi_rxulsch_ind_datapduinfo(tvb, pinfo, fapi_rxulsch_ind_datapduinfo_tree, data, offset, pdu_size - 4, &data_offset);
	}

        proto_tree *fapi_rxulsch_ind_pdubuffer_tree = proto_item_add_subtree(fapi_rxulsch_ind_item, ett_fapi_rxulsch_ind_pdubuffer);
        proto_tree_add_item(fapi_rxulsch_ind_pdubuffer_tree, hf_fapi_rxulsch_ind_pdubuffer, tvb, *offset, pdu_size - (4 + 16 * numOfPdu), ENC_NA);

        *offset = data_offset;

	return tvb_captured_length(tvb);
}

static int dissect_fapi_ulcrc_ind_crcpdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
    proto_item *fapi_ulcrc_ind_crcpdu_item = proto_tree_add_item(tree, hf_fapi_ulcrc_ind_crcpdu, tvb,  *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulcrc_ind_crcpdu_handle_tree = proto_item_add_subtree(fapi_ulcrc_ind_crcpdu_item, ett_fapi_ulcrc_ind_crcpdu_handle);
    proto_tree_add_item(fapi_ulcrc_ind_crcpdu_handle_tree, hf_fapi_ulcrc_ind_crcpdu_handle, tvb, *offset, 4, ENC_BIG_ENDIAN);

    *offset += 4;

    proto_tree *fapi_ulcrc_ind_crcpdu_rnti_tree = proto_item_add_subtree(fapi_ulcrc_ind_crcpdu_item, ett_fapi_ulcrc_ind_crcpdu_rnti);
    proto_tree_add_item(fapi_ulcrc_ind_crcpdu_rnti_tree, hf_fapi_ulcrc_ind_crcpdu_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

    *offset += 2;

    proto_tree *fapi_ulcrc_ind_crcpdu_crcflag_tree = proto_item_add_subtree(fapi_ulcrc_ind_crcpdu_item, ett_fapi_ulcrc_ind_crcpdu_crcflag);
    proto_tree_add_item(fapi_ulcrc_ind_crcpdu_crcflag_tree, hf_fapi_ulcrc_ind_crcpdu_crcflag, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    proto_tree *fapi_ulcrc_ind_crcpdu_padding_tree = proto_item_add_subtree(fapi_ulcrc_ind_crcpdu_item, ett_fapi_ulcrc_ind_crcpdu_padding);
    proto_tree_add_item(fapi_ulcrc_ind_crcpdu_padding_tree, hf_fapi_ulcrc_ind_crcpdu_padding, tvb, *offset, 1, ENC_NA);

    *offset += 1;

    return tvb_captured_length(tvb);
}

static int dissect_fapi_ulcrc_ind(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
    proto_item *fapi_ulcrc_ind_item = proto_tree_add_item(tree, hf_fapi_ulcrc_ind, tvb, *offset, pdu_size, ENC_NA);

    proto_tree *fapi_ulcrc_ind_sfnsf_tree = proto_item_add_subtree(fapi_ulcrc_ind_item, ett_fapi_ulcrc_ind_sfnsf);

        proto_item *fapi_ulcrc_ind_sfnsf_item = proto_tree_add_item(fapi_ulcrc_ind_sfnsf_tree, hf_fapi_ulcrc_ind_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_ulcrc_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_ulcrc_ind_sfnsf_item, ett_fapi_ulcrc_ind_sfnsf_sfn);
        proto_tree_add_item(fapi_ulcrc_ind_sfnsf_sfn_tree, hf_fapi_ulcrc_ind_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_ulcrc_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_ulcrc_ind_sfnsf_item, ett_fapi_ulcrc_ind_sfnsf_sf);
        proto_tree_add_item(fapi_ulcrc_ind_sfnsf_sf_tree, hf_fapi_ulcrc_ind_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

    proto_tree *fapi_ulcrc_ind_numofcrc_tree = proto_item_add_subtree(fapi_ulcrc_ind_item, ett_fapi_ulcrc_ind_numofcrc);
    proto_tree_add_item(fapi_ulcrc_ind_numofcrc_tree, hf_fapi_ulcrc_ind_numofcrc, tvb, *offset, 2, ENC_BIG_ENDIAN);

    guint16 numOfCrc = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

    *offset += 2;

    gint16 i;
    for (i = 0; i < numOfCrc; i++) {
	    proto_tree  *fapi_ulcrc_ind_crcpdu_tree = proto_item_add_subtree(fapi_ulcrc_ind_item, ett_fapi_ulcrc_ind_crcpdu);

	    dissect_fapi_ulcrc_ind_crcpdu(tvb, pinfo, fapi_ulcrc_ind_crcpdu_tree, data, offset, pdu_size - 4);
    }

    return tvb_captured_length(tvb);
}

static int dissect_fapi_dldatatx_req(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 numOfPdu;
	guint16 i;

        proto_item *fapi_dldatatx_req_item = proto_tree_add_item(tree, hf_fapi_dldatatx_req, tvb, *offset, pdu_size, ENC_NA);

        proto_tree *fapi_dldatatx_req_sfnsf_tree = proto_item_add_subtree(fapi_dldatatx_req_item, ett_fapi_subframe_ind_sfnsf);

        proto_item *fapi_dldatatx_req_sfnsf_item = proto_tree_add_item(fapi_dldatatx_req_sfnsf_tree, hf_fapi_dldatatx_req_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_dldatatx_req_sfnsf_sfn_tree = proto_item_add_subtree(fapi_dldatatx_req_sfnsf_item, ett_fapi_dldatatx_req_sfnsf_sfn);
        proto_tree_add_item(fapi_dldatatx_req_sfnsf_sfn_tree, hf_fapi_dldatatx_req_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_dldatatx_req_sfnsf_sf_tree = proto_item_add_subtree(fapi_dldatatx_req_sfnsf_item, ett_fapi_dldatatx_req_sfnsf_sf);
        proto_tree_add_item(fapi_dldatatx_req_sfnsf_sf_tree, hf_fapi_dldatatx_req_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_tree *fapi_dldatatx_req_numofpdu_tree = proto_item_add_subtree(fapi_dldatatx_req_item, ett_fapi_dldatatx_req_numofpdu);
        proto_tree_add_item(fapi_dldatatx_req_numofpdu_tree, hf_fapi_dldatatx_req_numofpdu, tvb, *offset, 2, ENC_BIG_ENDIAN);

	numOfPdu = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

        *offset += 2;

	for (i = 0; i < numOfPdu; i++) {
            proto_tree *fapi_dldatatx_req_dlpdu_info_tree = proto_item_add_subtree(fapi_dldatatx_req_item, ett_fapi_dldatatx_req_dlpdu_info);
            dissect_fapi_dldatatx_req_dlpdu_info(tvb, pinfo, fapi_dldatatx_req_dlpdu_info_tree, data, offset, pdu_size - 4);
	}
	return tvb_captured_length(tvb);
}

static int dissect_fapi_ulconfig_req(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 ulconfig_len = 0;
	gint8 i;
	gint8 numOfPdu;

	ulconfig_len = tvb_get_guint16(tvb, *offset + 2, ENC_BIG_ENDIAN);
        proto_item *fapi_ulconfig_req_item = proto_tree_add_item(tree, hf_fapi_ulconfig_req, tvb, *offset, ulconfig_len, ENC_NA);

        proto_tree *fapi_ulconfig_req_sfnsf_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_sfnsf);

        proto_item *fapi_ulconfig_req_sfnsf_item = proto_tree_add_item(fapi_ulconfig_req_sfnsf_tree, hf_fapi_ulconfig_req_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_ulconfig_req_sfnsf_sfn_tree = proto_item_add_subtree(fapi_ulconfig_req_sfnsf_item, ett_fapi_ulconfig_req_sfnsf_sfn);
        proto_tree_add_item(fapi_ulconfig_req_sfnsf_sfn_tree, hf_fapi_ulconfig_req_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_ulconfig_req_sfnsf_sf_tree = proto_item_add_subtree(fapi_ulconfig_req_sfnsf_item, ett_fapi_ulconfig_req_sfnsf_sf);
        proto_tree_add_item(fapi_ulconfig_req_sfnsf_sf_tree, hf_fapi_ulconfig_req_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_tree *fapi_ulconfig_req_ulconfiglen_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_ulconfiglen);
        proto_tree_add_item(fapi_ulconfig_req_ulconfiglen_tree, hf_fapi_ulconfig_req_ulconfiglen, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_item *fapi_ulconfig_req_numpdu_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_numpdu);
        proto_tree_add_item(fapi_ulconfig_req_numpdu_tree, hf_fapi_ulconfig_req_numpdu, tvb, *offset, 1, ENC_NA);

	numOfPdu = tvb_get_guint8(tvb, *offset);
        *offset += 1;

        proto_tree *fapi_ulconfig_req_rachfreqresources_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_rachfreqresources);
        proto_item *fapi_ulconfig_req_rachfreqresources_item = proto_tree_add_item(fapi_ulconfig_req_rachfreqresources_tree, hf_fapi_ulconfig_req_rachfreqresources, tvb, *offset, 1, ENC_NA);

	proto_tree *fapi_ulconfig_req_rachfreqresources_fdd_tree = 
		proto_item_add_subtree(fapi_ulconfig_req_rachfreqresources_item, ett_fapi_ulconfig_req_rachfreqresources_fdd);
	proto_tree_add_item(fapi_ulconfig_req_rachfreqresources_fdd_tree, hf_fapi_ulconfig_req_rachfreqresources_fdd, tvb, *offset, 1,  ENC_NA);

        *offset += 1;

        proto_tree *fapi_ulconfig_req_srs_present_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_srs_present);
        proto_tree_add_item(fapi_ulconfig_req_srs_present_tree, hf_fapi_ulconfig_req_srs_present, tvb, *offset, 1, ENC_NA);

        *offset += 1;
        
        proto_tree *fapi_ulconfig_req_padding_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_padding);
        proto_tree_add_item(fapi_ulconfig_req_padding_tree, hf_fapi_ulconfig_req_padding, tvb, *offset, 1, ENC_NA);

        *offset += 1;

	for (i = 0; i < numOfPdu; i++) {
		proto_tree *fapi_ulconfig_req_pdu_info_tree = proto_item_add_subtree(fapi_ulconfig_req_item, ett_fapi_ulconfig_req_pdu_info);

		dissect_fapi_ulconfig_pdu_info(tvb, pinfo, fapi_ulconfig_req_pdu_info_tree, data, offset);
	}

        return tvb_captured_length(tvb);
}

static int dissect_fapi_dlconfig_req(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_) 
{
	guint16 txPowerForPCFICH;
        guint16 i;
	guint16 numOfPDU;
	guint16 dlconfig_len = 0;

	dlconfig_len = tvb_get_guint16(tvb, *offset + 2, ENC_BIG_ENDIAN);

        proto_item *fapi_dlconfig_req_item = proto_tree_add_item(tree, hf_fapi_dlconfig_req, tvb, *offset, dlconfig_len, ENC_NA);

        proto_tree *fapi_dlconfig_req_sfnsf_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_sfnsf);

        proto_item *fapi_dlconfig_req_sfnsf_item = proto_tree_add_item(fapi_dlconfig_req_sfnsf_tree, hf_fapi_dlconfig_req_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_dlconfig_req_sfnsf_sfn_tree = proto_item_add_subtree(fapi_dlconfig_req_sfnsf_item, ett_fapi_dlconfig_req_sfnsf_sfn);
        proto_tree_add_item(fapi_dlconfig_req_sfnsf_sfn_tree, hf_fapi_dlconfig_req_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_dlconfig_req_sfnsf_sf_tree = proto_item_add_subtree(fapi_dlconfig_req_sfnsf_item, ett_fapi_dlconfig_req_sfnsf_sf);
        proto_tree_add_item(fapi_dlconfig_req_sfnsf_sf_tree, hf_fapi_dlconfig_req_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_tree* fapi_dlconfig_req_len_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_len);
        proto_tree_add_item(fapi_dlconfig_req_len_tree, hf_fapi_dlconfig_req_len, tvb, *offset, 2, ENC_BIG_ENDIAN);
        
        *offset += 2;

        proto_tree *fapi_dlconfig_req_cfi_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_cfi);
        proto_tree_add_item(fapi_dlconfig_req_cfi_tree, hf_fapi_dlconfig_req_cfi, tvb, *offset, 1, ENC_NA);

        *offset += 1;

        proto_tree *fapi_dlconfig_req_numDCI_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_numDCI);
        proto_tree_add_item(fapi_dlconfig_req_numDCI_tree, hf_fapi_dlconfig_req_numDCI, tvb, *offset, 1, ENC_NA);

        *offset += 1;

        proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_numOfPDU);
        proto_tree_add_item(fapi_dlconfig_req_item, hf_fapi_dlconfig_req_numOfPDU, tvb, *offset, 2, ENC_BIG_ENDIAN);

        numOfPDU = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_tree *fapi_dlconfig_req_txpowerForPCFICH_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_txPowerForPCFICH);
        proto_item *fapi_dlconfig_req_txPowerForPCFICH_item = 
                proto_tree_add_item(fapi_dlconfig_req_txpowerForPCFICH_tree, hf_fapi_dlconfig_req_txPowerForPCFICH, tvb, *offset, 2, ENC_BIG_ENDIAN);

        txPowerForPCFICH =  tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);
        proto_item_append_text(fapi_dlconfig_req_txPowerForPCFICH_item, " (%g dBm)", (float)(txPowerForPCFICH - 6000)/1000);

        *offset += 2;

        proto_tree *fapi_dlconfig_req_numOfPDSCHRNTI_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_numOfPDSCHRNTI);
        proto_tree_add_item(fapi_dlconfig_req_numOfPDSCHRNTI_tree, hf_fapi_dlconfig_req_numOfPDSCHRNTI, tvb, *offset, 1, ENC_NA);

        *offset += 1;

        proto_tree *fapi_dlconfig_req_padding_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_padding);
        proto_tree_add_item(fapi_dlconfig_req_padding_tree, hf_fapi_dlconfig_req_padding, tvb, *offset, 1, ENC_NA);

        *offset += 1;
        
        for (i = 0; i < numOfPDU; i++) {
            proto_tree *fapi_dlconfig_req_pdu_info_tree = proto_item_add_subtree(fapi_dlconfig_req_item, ett_fapi_dlconfig_req_pdu_info);

            dissect_fapi_dlconfig_pdu_info(tvb, pinfo, fapi_dlconfig_req_pdu_info_tree, data, offset);
        }

	/* 
	 * There seems to be an issue with encoding of the size of dl config request
	 *
	 * if (numOfPDU) *offset += 4; 
	 */
        return tvb_captured_length(tvb);
}
 
static int dissect_fapi_hidci0_hidc0pdu_info_hipdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_)
{
	proto_item *fapi_hidci0_hidci0pdu_info_hipdu_item = proto_tree_add_item(tree, hf_fapi_hidci0_hidci0pdu_info_hipdu, tvb, *offset, FAPI_HIDCI0_HIDCI0PDU_INFO_HIPDU_SIZE, ENC_NA); 

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_pdutype_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_pdutype);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_pdutype_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_pdutype, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_hipdusize_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_hipdusize);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_hipdusize_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_hipdusize, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_rbstart_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_rbstart);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_rbstart_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_rbstart, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_hivalue_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_hivalue);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_hivalue_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_hivalue, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_iphich_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_iphich);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_iphich_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_iphich, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hipdu_txpower_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_hipdu_item, ett_fapi_hidci0_hidci0pdu_info_hipdu_txpower);
	proto_item* fapi_hidci0_hidci0pdu_info_hipdu_txpower_item = 
	       	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hipdu_txpower_tree, hf_fapi_hidci0_hidci0pdu_info_hipdu_txpower, tvb, *offset, 2, ENC_BIG_ENDIAN);

	guint16 txPower = tvb_get_guint16(tvb, *offset, ENC_BIG_ENDIAN);

        proto_item_append_text(fapi_hidci0_hidci0pdu_info_hipdu_txpower_item, " (%g dBm)", (float)(txPower - 6000)/1000);
	*offset += 2;

	return tvb_captured_length(tvb);
}

static int dissect_fapi_hidci0_hidc0pdu_info_hidci0pdu(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_)
{
	proto_item *fapi_hidci0_hidci0pdu_info_dci0pdu_item = proto_tree_add_item(tree, hf_fapi_hidci0_hidci0pdu_info_hidci0, tvb, *offset, FAPI_HIDCI0_HIDCI0PDU_INFO_DCI0PDU_SIZE, ENC_NA); 

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_pdutype_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_pdutype);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_pdutype_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_pdutype, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_dciformat_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_dciformat);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_dciformat_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_dciformat, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_cceindex_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_cceindex);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_cceindex_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_cceindex, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_rnti_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_rnti);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_rnti_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_rnti, tvb, *offset, 2, ENC_BIG_ENDIAN);

	*offset += 2;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_agglevel_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_agglevel);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_agglevel_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_agglevel, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_rbstart_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_rbstart);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_rbstart_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_rbstart, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_numofrb_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_numofrb);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_numofrb_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_numofrb, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_mcs_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_mcs);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_mcs_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_mcs, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_newdataindication_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_newdataindication);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_newdataindication_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_newdataindication, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_tpc_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_tpc);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_tpc_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_tpc, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_cqirequest_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_cqirequest);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_cqirequest_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_cqirequest, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_ulindex_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_ulindex);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_ulindex_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_ulindex, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_padding_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_padding);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_padding_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_padding, tvb, *offset, 1, ENC_NA);

	*offset += 1;

	proto_tree* fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_dci0pdu_item, ett_fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap);
	proto_tree_add_item(fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap_tree, hf_fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap, tvb, *offset, 4, ENC_BIG_ENDIAN);

	*offset += 4;

	return tvb_captured_length(tvb);
}

static int dissect_fapi_hidci0_ind(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_)
{
	proto_item *fapi_hidci0_ind_item = proto_tree_add_item(tree, hf_fapi_hidci0_ind, tvb, *offset, pdu_size, ENC_NA);

	proto_tree *fapi_hidci0_ind_sfnsf_tree = proto_item_add_subtree(fapi_hidci0_ind_item, ett_fapi_hidci0_sfnsf);

        proto_item *fapi_hidci0_ind_sfnsf_item = proto_tree_add_item(fapi_hidci0_ind_sfnsf_tree, hf_fapi_hidci0_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_hidci0_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_hidci0_ind_sfnsf_item, ett_fapi_hidci0_sfnsf_sfn);
        proto_tree_add_item(fapi_hidci0_ind_sfnsf_sfn_tree, hf_fapi_hidci0_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_hidci0_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_hidci0_ind_sfnsf_item, ett_fapi_hidci0_sfnsf_sf);
        proto_tree_add_item(fapi_hidci0_ind_sfnsf_sf_tree, hf_fapi_hidci0_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;
	
	proto_tree *fapi_hidci0_ind_numdci_tree = proto_item_add_subtree(fapi_hidci0_ind_item, ett_fapi_hidci0_numofdci);
	proto_tree_add_item(fapi_hidci0_ind_numdci_tree, hf_fapi_hidci0_numofdci, tvb, *offset, 1, ENC_NA);

        guint8 numOfDci0 = tvb_get_guint8(tvb, *offset);

	*offset += 1;

	proto_tree *fapi_hidci0_ind_numhi_tree = proto_item_add_subtree(fapi_hidci0_ind_item, ett_fapi_hidci0_numofhi);
	proto_tree_add_item(fapi_hidci0_ind_numhi_tree, hf_fapi_hidci0_numofhi, tvb, *offset, 1, ENC_NA);

        guint8 numOfHi = tvb_get_guint8(tvb, *offset);

	*offset += 1;


        gint8 i;
        for (i = 0; i < numOfDci0 + numOfHi; i++) {
		proto_tree *fapi_hidci0_hidci0pdu_info_tree = proto_item_add_subtree(fapi_hidci0_ind_item, ett_fapi_hidci0_hidci0pdu_info);
		proto_item *fapi_hidci0_hidci0pdu_info_item = proto_tree_add_item(fapi_hidci0_hidci0pdu_info_tree, hf_fapi_hidci0_hidci0pdu_info, tvb, *offset, pdu_size - 4, ENC_NA);

                guint8 pduType = tvb_get_guint8(tvb, *offset);

                switch (pduType) {
                	case 0: { /* HI PDU */
	                	proto_tree *fapi_hidci0_hidci0pdu_info_hipdu_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_item, ett_fapi_hidci0_hidci0pdu_info_hipdu);
                		dissect_fapi_hidci0_hidc0pdu_info_hipdu(tvb, pinfo, fapi_hidci0_hidci0pdu_info_hipdu_tree, data, offset, pdu_size - 4);
                	}
                	break;
                	case 1: { /* UL DCI PDU */
	                	proto_tree *fapi_hidci0_hidci0pdu_info_dci0pdu_tree = proto_item_add_subtree(fapi_hidci0_hidci0pdu_info_item, ett_fapi_hidci0_hidci0pdu_info_hidci0);
                		dissect_fapi_hidci0_hidc0pdu_info_hidci0pdu(tvb, pinfo, fapi_hidci0_hidci0pdu_info_dci0pdu_tree, data, offset, pdu_size - 4);
                	}
                	break;
                } /* switch (pduType) */
        }

	return tvb_captured_length(tvb);
}
static int dissect_fapi_subframe_ind(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_, guint *offset _U_, guint pdu_size _U_)
{
        proto_item *fapi_subframe_ind_item = proto_tree_add_item(tree, hf_fapi_subframe_ind, tvb, *offset, 4, ENC_NA);

        proto_tree *fapi_subframe_ind_sfnsf_tree = proto_item_add_subtree(fapi_subframe_ind_item, ett_fapi_subframe_ind_sfnsf);

        proto_item *fapi_subframe_ind_sfnsf_item = proto_tree_add_item(fapi_subframe_ind_sfnsf_tree, hf_fapi_subframe_ind_sfnsf, tvb, *offset, 2, ENC_NA);

        proto_tree *fapi_subframe_ind_sfnsf_sfn_tree = proto_item_add_subtree(fapi_subframe_ind_sfnsf_item, ett_fapi_subframe_ind_sfnsf_sfn);
        proto_tree_add_item(fapi_subframe_ind_sfnsf_sfn_tree, hf_fapi_subframe_ind_sfnsf_sfn, tvb, *offset, 2, ENC_BIG_ENDIAN);

        proto_tree *fapi_subframe_ind_sfnsf_sf_tree = proto_item_add_subtree(fapi_subframe_ind_sfnsf_item, ett_fapi_subframe_ind_sfnsf_sf);
        proto_tree_add_item(fapi_subframe_ind_sfnsf_sf_tree, hf_fapi_subframe_ind_sfnsf_sf, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        proto_tree *fapi_subframe_ind_padding_tree = proto_item_add_subtree(fapi_subframe_ind_item, ett_fapi_subframe_ind_padding);
        proto_tree_add_item(fapi_subframe_ind_padding_tree, hf_fapi_subframe_ind_padding, tvb, *offset, 2, ENC_BIG_ENDIAN);

        *offset += 2;

        return tvb_captured_length(tvb);
}

static int dissect_fapi(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
    guint8 msg_id = tvb_get_guint8(tvb, 0);
    guint     offset = 0;
    guint8 last_msg_id = 0xff;

    col_set_str(pinfo->cinfo, COL_PROTOCOL, "FAPI");
    col_clear(pinfo->cinfo,COL_INFO);
    col_append_fstr(pinfo->cinfo, COL_INFO, " %s ", "FAPI Bundle");

     while (last_msg_id != msg_id) {
    
        const gchar* message_str = val_to_str_const(msg_id, message_id_vals, "Unknown (0x%02x)");
    
        guint16 msg_len = tvb_get_guint16(tvb, offset + 2, ENC_BIG_ENDIAN);
    
        proto_item *fapi_message_item = proto_tree_add_item(tree, proto_fapi, tvb, offset, msg_len + 4, ENC_NA); // 4 bytes for header
    
        proto_item_append_text(fapi_message_item, " (%s)", message_str); /* sneak peek */
     
        proto_tree *fapi_message_type = proto_item_add_subtree(fapi_message_item, ett_fapi_message_type);
        proto_tree_add_item(fapi_message_type, hf_fapi_message_type, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset += 1;
    
        proto_tree *fapi_message_vendor_tlv_len = proto_item_add_subtree(fapi_message_item, ett_fapi_message_vendor_tlv_len);
        proto_tree_add_item(fapi_message_vendor_tlv_len, hf_fapi_message_vendor_tlv_len, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset += 1;
    
        proto_tree *fapi_message_len = proto_item_add_subtree(fapi_message_item, ett_fapi_message_len);
        proto_tree_add_item(fapi_message_len, hf_fapi_message_len, tvb, offset, 2, ENC_BIG_ENDIAN);
    
        offset += 2;
    
        proto_tree *fapi_message_body = proto_item_add_subtree(fapi_message_item, ett_fapi_message_body);
        /*
         */
	guint loffset = offset;
        switch (msg_id) {
            case 0x80:
                    dissect_fapi_dlconfig_req(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
                    break;
            case 0x81:
                    dissect_fapi_ulconfig_req(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
                    break;
            case 0x82:
                    dissect_fapi_subframe_ind(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
                    break;
	    case 0x83:
		    dissect_fapi_hidci0_ind(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
		    break;
	    case 0x84:
		    dissect_fapi_dldatatx_req(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
		    break;
	    case 0x85:
		    dissect_fapi_ulharq_ind(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
		    break;
            case 0x86:
		    dissect_fapi_ulcrc_ind(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
		    break;
	    case 0x87:
		    dissect_fapi_rxulsch_ind(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
		    break;
	    case 0x88:
		    dissect_fapi_rach_ind(tvb, pinfo, fapi_message_body, data, &loffset, msg_len);
		    break;
            default: {
                    proto_item *fapi_message_body_item = proto_tree_add_item(fapi_message_body, hf_fapi_message_body, tvb, offset, msg_len, ENC_NA);

                    proto_item_add_subtree(fapi_message_body_item, ett_fapi_message_body);
            }
            break;
        } 

        offset += msg_len;

        last_msg_id = msg_id;

        msg_id = tvb_get_guint8(tvb, offset);
    } 

    return tvb_captured_length(tvb);
}

void
proto_register_fapi(void)
{
    static hf_register_info hf[] = {
        { &hf_fapi_message_type, {"type", "fapi.message_type", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_message_vendor_tlv_len, {"vendor tlv size", "fapi.message_vendor_tlv_len", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL} },
        { &hf_fapi_message_len, {"size", "fapi.message_len", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },

        { &hf_fapi_message_body, {"payload", "fapi.message_body", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

        { &hf_fapi_subframe_ind, {"subframe indication", "fapi.subframe_ind", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_subframe_ind_sfnsf, {"sfnsf", "fapi.subframe_ind.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_subframe_ind_sfnsf_sfn, {"sfn", "fapi.subframe_ind.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
        { &hf_fapi_subframe_ind_sfnsf_sf, {"sf", "fapi.subframe_ind.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
        { &hf_fapi_subframe_ind_padding, {"padding", "fapi.subframe_ind.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

        { &hf_fapi_dlconfig_req, {"dl config request", "fapi.dlconfig_req", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_sfnsf, {"sfnsf", "fapi.dlconfig_req.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_sfnsf_sfn, {"sfn", "fapi.dlconfig_req.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_sfnsf_sf, {"sf", "fapi.dlconfig_req.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_len, {"length", "fapi.dlconfig_req.len", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_cfi, {"cfi", "fapi.dlconfig_req.cfi", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_numDCI, {"numDCI", "fapi.dlconfig_req.numDCI", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_numOfPDU, {"numOfPDU", "fapi.dlconfig_req.numOfPDU", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_txPowerForPCFICH, {"txPowerForPCFICH", "fapi.dlconfig_req.txPowerForPCFICH", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_numOfPDSCHRNTI, {"numOfPDSCHRNTI", "fapi.dlconfig_req.numOfPDSCHRNTI", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_dlconfig_req_padding, {"padding", "fapi.dlconfig_req.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info,{"dlConfigPDUInfo", "fapi.dlconfig_req.dlConfigPDUInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pdutype, {"pduType", "fapi.dlconfig_req.dlConfigPDUInfo.pduType", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_pdutype_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pdusize, {"pduSize", "fapi.dlconfig_req.dlConfigPDUInfo.pduSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pdupadding, {"padding", "fapi.dlconfig_req.dlConfigPDUInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion, {"dlConfigPDUInfoUnion", "fapi.dlconfig_req.dlConfigPDUInfo.union", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu, {"DCIPdu", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dciformat, {"dciFormat", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciFormat", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dciformat_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_cceIndex, {"cceIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.cceIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_rnti, {"rnti", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu, {"dciPdu", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1, {"dci 1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlvl, {"aggregationLevel", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.aggregationLevel", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype, {"resAllocationType", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.vrbAssignment", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dci1_resallocationtype_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_mcs, {"mcs_1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.mcs_1", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rv, {"redundancyVersion_1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.redundancyVersion_1", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding, {"rbCoding", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.rbCoding", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_ndi, {"newDataIndicator_1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.newDataIndicator_1", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc, {"harqProcessNum", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.harqProcessNum", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_tpc, {"tpc", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.tpc", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_dai, {"dlAssignmentIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.dlAssignmentIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_txpower, {"txPower", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.txPower", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype, {"rntiType", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.rntiType", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dci1a_rntitype_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_padding, {"padding", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a, {"dci 1A", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlvl, {"aggregationLevel", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.aggregationLevel", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment, {"vrbAssignment", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.vrbAssignment", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dci1a_vrbassignment_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs, {"mcs_1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.mcs_1", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rv, {"redundancyVersion_1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.redundancyVersion_1", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding, {"rbCoding", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.rbCoding", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi, {"newDataIndicator_1", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.newDataIndicator_1", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc, {"harqProcessNum", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.harqProcessNum", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc, {"tpc", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.tpc", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_dai, {"dlAssignmentIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.dlAssignmentIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach, {"allocatePRACHFlag", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.allocatePRACHFlag", FT_BOOLEAN, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx, {"preambleIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.preambleIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower, {"txPower", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.txPower", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx, {"PRACHMaskIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.pRACHMaskIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype, {"rntiType", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.rntiType", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dci1a_rntitype_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_padding, {"padding", "fapi.dlconfig_req.dlConfigPDUInfo.union.DCIPdu.dciPdu.1A.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu, {"DLSCHPdu", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pdulen, {"dlschPduLen", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.dlschPduLen", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pduidx, {"pduIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.pduIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rnti, {"rnti", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_resallocationtype, {"resAllocationType", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.resAllocationType", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dci1_resallocationtype_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_vrbassignment, {"vrbAssignmentFlag", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.vrbAssignmentFlag", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dci1a_vrbassignment_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rbcoding, {"rbCoding", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.rbCoding", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_mcs, {"mcs", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.mcs", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rv, {"redundancyVersion", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.redundancyVersion", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transportblocks, {"transportBlocks", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.transportBlocks", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_tb2cwswapflag, {"tbToCodeWordSwapFlag", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.tbToCodeWordSwapFlag", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transmissionscheme, {"transmissionScheme", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.transmissionScheme", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dlsch_txscheme_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numlayers, {"numOfLayers", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.numOfLayers", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numsubbands, {"numOfSubBands", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.numOfSubBands", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_uecategory, {"ueCategoryCapacity", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.ueCategoryCapacity", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pa, {"pA", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.pA", FT_UINT8, BASE_DEC, (const void *)&dlconfig_req_dlsch_pa_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex, {"deltaPowerOffsetAIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.deltaPowerOffsetAIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_ngap, {"nGap", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.nGap", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_nprb, {"nPRB", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.nPRB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numrbpersubband, {"numRbPerSubBand", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.numRbPerSubBand", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numbfVector, {"numBfVector", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.numBfVector", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_subbandInfo, {"subBandInfo", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.subBandInfo", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo, {"bfVector", "fapi.dlconfig_req.dlConfigPDUInfo.union.DLSCHPdu.bfVector", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu, {"BCHPdu", "fapi.dlconfig_req.dlConfigPDUInfo.union.BCHPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pdulen, {"bchPduLen", "fapi.dlconfig_req.dlConfigPDUInfo.union.BCHPdu.bchPduLen", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pduidx, {"pduIndex", "fapi.dlconfig_req.dlConfigPDUInfo.union.BCHPdu.pduIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_txpower, {"txPower", "fapi.dlconfig_req.dlConfigPDUInfo.union.BCHPdu.txPower", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_padding, {"padding", "fapi.dlconfig_req.dlConfigPDUInfo.union.BCHPdu.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

        { &hf_fapi_ulconfig_req, {"ul config request", "fapi.ulconfig_req", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_sfnsf, {"sfnsf", "fapi.ulconfig_req.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_sfnsf_sfn, {"sfn", "fapi.ulconfig_req.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_sfnsf_sf, {"sf", "fapi.ulconfig_req.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_ulconfiglen, {"length", "fapi.ulconfig_req.len", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_numpdu, {"numOfPDU", "fapi.ulconfig_req.numOfPDU", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_rachfreqresources, {"rachFreqResources", "fapi.ulconfig_req.rachFreqResouces", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_rachfreqresources_fdd, {"rachFreqResoucesFDD", "fapi.ulconfig_req.rachFreqResouces.fdd", FT_BOOLEAN, BASE_NONE, NULL, 0x01, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_srs_present, {"srsPresent", "fapi.ulconfig_req.srsPresent", FT_BOOLEAN, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_padding, {"padding", "fapi.ulconfig_req.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info, {"pduConfigInfo", "fapi.ulconfig_req.pduConfigInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pdutype, {"pduType", "fapi.ulconfig_req.pduConfigInfo.pduType", FT_UINT8, BASE_DEC, (const void *)&ulconfig_req_pdutype_vals, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pdusize, {"pduSize", "fapi.ulconfig_req.pduConfigInfo.pduSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo, {"pduConfig", "fapi.ulconfig_req.pduConfigInfo.pduConfig", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },

        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu, {"cqiPdu", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_handle, {"handle", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_rnti, {"rnti", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_length, {"length", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.length", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_dataoffset, {"dataOffset", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.dataOffset", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_timingadvance, {"timingAdvance", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.timingAdvance", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ulcqi, {"ulCqi", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.ulCqi", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ri, {"ri", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.ri", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.pduConfig.cqiPdu.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu, {"harqPdu", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_handle, {"handle", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_rnti, {"rnti", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo, {"harqInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.harqInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex, {"pucchIndex", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.harqInfo.pucchIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1, {"pucchIndex1", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.harqInfo.pucchIndex1", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize, {"harqSize", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.harqInfo.harqSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.pduConfig.harqPdu.harqInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu, {"srHarqPdu", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_handle, {"handle", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_rnti, {"rnti", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo, {"srInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.srInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex, {"pucchIndex", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.srInfo.pucchIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo, {"harqInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.harqInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex, {"pucchIndex", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.harqInfo.pucchIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1, {"pucchIndex1", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.harqInfo.pucchIndex1", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize, {"harqSize", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.harqInfo.harqSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srHarqPdu.harqInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu, {"srPdu", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_handle, {"handle", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srPdu.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_rnti, {"rnti", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo, {"srInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srPdu.srInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex, {"pucchIndex", "fapi.ulconfig_req.pduConfigInfo.pduConfig.srPdu.srInfo.pucchIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },


        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu, {"ulSchPdu", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_handle, {"handle", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_size, {"size", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.size", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rnti, {"rnti", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rbstart, {"rbStart", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.rbStart", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_numofrb, {"numOfRB", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.numOfRB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_modulationtype, {"modulationType", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.modulationType", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs, {"cyclicShift2ForDMRS", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.cyclicShift2ForDMRS", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled, {"freqHoppingEnabledFlag", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.freqHoppingEnabledFlag", FT_BOOLEAN, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits, {"frequencyHoppingBits", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.frequencyHoppingBits", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_ndi, {"newDataIndication", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.newDataIndication", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rv, {"redundancyVersion", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.redundancyVersion", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_harqproc, {"harqProcessNumber", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.harqProcessNumber", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_txmode, {"ulTxMode", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.ulTxMode", FT_UINT8, BASE_DEC, (const void *)ulconfig_req_ulsch_txmode_vals, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_currtxnb, {"currTxNB", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.currTxNB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_nsrs, {"nSRS", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchPdu.nSRS", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu, {"ulSchHarqPdu", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo, {"ulSchPduInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle, {"handle", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size, {"size", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.size", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti, {"rnti", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart, {"rbStart", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.rbStart", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb, {"numOfRB", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.numOfRB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype, {"modulationType", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.modulationType", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs, {"cyclicShift2ForDMRS", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.cyclicShift2ForDMRS", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled, {"freqHoppingEnabledFlag", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.freqHoppingEnabledFlag", FT_BOOLEAN, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits, {"frequencyHoppingBits", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.frequencyHoppingBits", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi, {"newDataIndication", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.newDataIndication", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv, {"redundancyVersion", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.redundancyVersion", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc, {"harqProcessNumber", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.harqProcessNumber", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode, {"ulTxMode", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.ulTxMode", FT_UINT8, BASE_DEC, (const void *)ulconfig_req_ulsch_txmode_vals, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb, {"currTxNB", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.currTxNB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs, {"nSRS", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.ulSchPduInfo.nSRS", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo, {"harqInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.harqInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize, {"harqSize", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.harqInfo.harqSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq, {"deltaOffsetHarq", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.harqInfo.deltaOffsetHarq", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode, {"ackNackMode", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.harqInfo.ackNackMode", FT_UINT8, BASE_DEC, (const void *)&ulconfig_req_harqinfo_acknackmode_vals, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.harqInfo.padding", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo, {"initialTxParamInfo", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.initialTxParamInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial, {"nSRSInitial", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.initialTxParamInfo.nSRSInitial", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb, {"initialNumOfRB", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.initialTxParamInfo.initialNumOfRB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
        { &hf_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding, {"padding", "fapi.ulconfig_req.pduConfigInfo.pduConfig.ulSchHarqPdu.initialTxParamInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_dldatatx_req, {"dl data tx req", "fapi.dldatatx_req", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_sfnsf, {"sfnsf", "fapi.dldatatx_req.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_sfnsf_sfn, {"sfn", "fapi.dldatatx_req.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_sfnsf_sf, {"sf", "fapi.dldatatx_req.sfnsfsf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_numofpdu, {"numOfPdu", "fapi.dldatatx_req.numOfPdu", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info, {"dlPduInfo", "fapi.dldatatx_req.dlPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_pdulen, {"pduLen", "fapi.dldatatx_req.dlPduInfo.pduLen", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_pduidx, {"pduIndex", "fapi.dldatatx_req.dlPduInfo.pduIndex", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_numoftlv, {"numOfTLV", "fapi.dldatatx_req.dlPduInfo.numOfTLV", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_tlvinfo, {"dlTLVInfo", "fapi.dldatatx_req.dlPduInfo.dlTLVInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_tag, {"tag", "fapi.dldatatx_req.dlPduInfo.dlTLVInfo.tag", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen, {"tagLen", "fapi.dldatatx_req.dlPduInfo.dlTLVInfo.tagLen", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_padding, {"padding", "fapi.dldatatx_req.dlPduInfo.dlTLVInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_dldatatx_req_dlpdu_info_tlvinfo_value, {"value", "fapi.dldatatx_req.dlPduInfo.dlTLVInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_rxulsch_ind, {"ul data rx ind", "fapi.rxulsch_ind", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_sfnsf, {"sfnsf", "fapi.rxulsch_ind.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_sfnsf_sfn, {"sfn", "fapi.rxulsch_ind.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_sfnsf_sf, {"sf", "fapi.rxulsch_ind.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_numofpdu, {"numOfPdu", "fapi.rxulsch_ind.numOfPdu", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo, {"dataPduInfo", "fapi.rxulsch_ind.dataPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_handle, {"handle", "fapi.rxulsch_ind.dataPduInfo.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_rnti, {"rnti", "fapi.rxulsch_ind.dataPduInfo.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_length, {"length", "fapi.rxulsch_ind.dataPduInfo.length", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_dataoffset, {"dataOffset", "fapi.rxulsch_ind.dataPduInfo.dataOffset", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_timingadvance, {"timingAdvance", "fapi.rxulsch_ind.dataPduInfo.timingAdvance", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_ulcqi, {"ulCqi", "fapi.rxulsch_ind.dataPduInfo.ulCqi", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_padding, {"padding", "fapi.rxulsch_ind.dataPduInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_datapduinfo_data, {"data", "fapi.rxulsch_ind.dataPduInfo.data", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rxulsch_ind_pdubuffer, {"pduBuffer", "fapi.rxulsch_ind.pduBuffer", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_ulcrc_ind, {"ul crc ind", "fapi.ulcrc_ind", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_sfnsf, {"sfnsf", "fapi.ulcrc_ind.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_sfnsf_sfn, {"sfn", "fapi.ulcrc_ind.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_sfnsf_sf, {"sf", "fapi.ulcrc_ind.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_numofcrc, {"numOfCrc", "fapi.ulcrc_ind.numOfCrc", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_crcpdu, {"crcPduInfo", "fapi.ulcrc_ind.crcPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_crcpdu_handle, {"handle", "fapi.ulcrc_ind.crcPduInfo.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_crcpdu_rnti, {"rnti", "fapi.ulcrc_ind.crcPduInfo.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_crcpdu_crcflag, {"crcFlag", "fapi.ulcrc_ind.crcPduInfo.crcflag", FT_UINT8, BASE_DEC, (const void *)&ulcrc_ind_crcflag_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulcrc_ind_crcpdu_padding, {"padding", "fapi.ulcrc_ind.crcPduInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_hidci0_ind, {"ul hidci0 ind", "fapi.hidci0_ind", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_sfnsf, {"sfnsf", "fapi.hidci0_ind.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_sfnsf_sfn, {"sfn", "fapi.hidci0_ind.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_hidci0_sfnsf_sf, {"sf", "fapi.hidci0_ind.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
	{ &hf_fapi_hidci0_numofdci, {"numOfDCI", "fapi.hidci0_ind.numOfDCI", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_numofhi, {"numOfHI", "fapi.hidci0_ind.numOfHI", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info, {"hidciPduInfo", "fapi.hidci0_ind.hidciPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu, {"hiPdu", "fapi.hidci0_ind.hidciPduInfo.hiPdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_pdutype, {"pduType", "fapi.hidci0_ind.hidciPduInfo.hiPdu.pduType", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_hipdusize, {"hipduSize", "fapi.hidci0_ind.hidciPduInfo.hiPdu.hiPduSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_rbstart, {"rbStart", "fapi.hidci0_ind.hidciPduInfo.hiPdu.rbStart", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs, {"cyclicShift2ForDMRS", "fapi.hidci0_ind.hidciPduInfo.hiPdu.cyclicShift2ForDMRS", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_hivalue, {"hiValue", "fapi.hidci0_ind.hidciPduInfo.hiPdu.hiValue", FT_UINT8, BASE_DEC, (const void *)&hidci0_ind_hivalue_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_iphich, {"iPHICH", "fapi.hidci0_ind.hidciPduInfo.hiPdu.iPHICH", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hipdu_txpower, {"txPower", "fapi.hidci0_ind.hidciPduInfo.hiPdu.txPower", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0, {"dci0Pdu", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_pdutype, {"pduType", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.pduType", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize, {"ulDciPduSize", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.ulDciPduSize", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_dciformat, {"ulDciFormat", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.ulDciFormat", FT_UINT8, BASE_DEC, (const void *)&hidci0_ind_dciformat_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_cceindex, {"cceIndex", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.cceIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_rnti, {"rnti", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_agglevel, {"aggLevel", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.aggLevel", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_rbstart, {"rbStart", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.rbStart", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_numofrb, {"numOfRB", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.numOfRB", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_mcs, {"mcs", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.mcs", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs, {"cyclicShift2ForDMRS", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.cyclicShift2ForDMRS", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag, {"freqEnabledFlag", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.freqEnabledFlag", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits, {"freqHoppingBits", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.freqHoppingBits", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_newdataindication, {"newDataIndication", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.newDataIndication", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection, {"ueTxAntennaSelection", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.ueTxAntennaSelection", FT_UINT8, BASE_DEC, (const void *)&hidci0_ind_uetxantennaselection_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_tpc, {"tpc", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.tpc", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_cqirequest, {"cqiRequest", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.cqiRequest", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_ulindex, {"ulIndex", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.ulIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex, {"dlAssignmentIndex", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.dlAssignmentIndex", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_padding, {"padding", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap, {"tpcBitMap", "fapi.hidci0_ind.hidciPduInfo.dci0Pdu.tpcBitMap", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_rach_ind, {"ul rach indication", "fapi.rach_ind", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_sfnsf, {"sfnsf", "fapi.rach_ind.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_sfnsf_sfn, {"sfn", "fapi.rach_ind.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_sfnsf_sf, {"sf", "fapi.rach_ind.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
	{ &hf_fapi_rach_ind_numofpreambles, {"numOfPreambles", "fapi.rach_ind.numOfPreambles", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_rachpdu_info, {"rachPduInfo", "fapi.rach_ind.rachPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_rachpdu_info_rnti, {"rnti", "fapi.rach_ind.rachPduInfo.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_rachpdu_info_timingadvance, {"timingAdvance", "fapi.rach_ind.rachPduInfo.timingAdvance", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_rachpdu_info_preamble, {"preamble", "fapi.rach_ind.rachPduInfo.preamble", FT_UINT8, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_rach_ind_rachpdu_info_padding, {"padding", "fapi.rach_ind.rachPduInfo.padding", FT_BYTES, BASE_NONE, NULL, 0x0, NULL, HFILL } },

	{ &hf_fapi_ulharq_ind, {"ul harq indication", "fapi.ulharq_ind", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_sfnsf, {"sfnsf", "fapi.ulharq_ind.sfnsf", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_sfnsf_sfn, {"sfn", "fapi.ulharq_ind.sfnsf.sfn", FT_UINT16, BASE_DEC, NULL, 0xfff0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_sfnsf_sf, {"sf", "fapi.ulharq_ind.sfnsf.sf", FT_UINT16, BASE_DEC, NULL, 0x000f, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_numofharq, {"numOfHarq", "fapi.ulharq_ind.numOfHarq", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_harqpdu_info, {"harqPduInfo", "fapi.ulharq_ind.harqPduInfo", FT_NONE, BASE_NONE, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_harqpdu_info_handle, {"handle", "fapi.ulharq_ind.harqPduInfo.handle", FT_UINT32, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_harqpdu_info_rnti, {"rnti", "fapi.ulharq_ind.harqPduInfo.rnti", FT_UINT16, BASE_DEC, NULL, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_harqpdu_info_harqtb1, {"harqTB1", "fapi.ulharq_ind.harqPduInfo.harqTB1", FT_UINT8, BASE_DEC, (const void *)&ulharq_ind_harqpdu_info_harq_vals, 0x0, NULL, HFILL } },
	{ &hf_fapi_ulharq_ind_harqpdu_info_harqtb2, {"harqTB2", "fapi.ulharq_ind.harqPduInfo.harqTB2", FT_UINT8, BASE_DEC, (const void *)&ulharq_ind_harqpdu_info_harq_vals, 0x0, NULL, HFILL } },
    };

    static gint *ett[] = {
        &ett_fapi_message_type,
        &ett_fapi_message_vendor_tlv_len,
        &ett_fapi_message_len,

        &ett_fapi_message_body,

        &ett_fapi_subframe_ind,
        &ett_fapi_subframe_ind_sfnsf,
        &ett_fapi_subframe_ind_sfnsf_sfn,
        &ett_fapi_subframe_ind_sfnsf_sf,
        &ett_fapi_subframe_ind_padding,

        &ett_fapi_dlconfig_req,
        &ett_fapi_dlconfig_req_sfnsf,
        &ett_fapi_dlconfig_req_sfnsf_sfn,
        &ett_fapi_dlconfig_req_sfnsf_sf,
        &ett_fapi_dlconfig_req_len,
        &ett_fapi_dlconfig_req_cfi,
        &ett_fapi_dlconfig_req_numDCI,
        &ett_fapi_dlconfig_req_numOfPDU,
        &ett_fapi_dlconfig_req_txPowerForPCFICH,
        &ett_fapi_dlconfig_req_numOfPDSCHRNTI,
        &ett_fapi_dlconfig_req_padding,

	&ett_fapi_dlconfig_req_pdu_info,
	&ett_fapi_dlconfig_req_pdu_info_pdutype,
	&ett_fapi_dlconfig_req_pdu_info_pdusize,
	&ett_fapi_dlconfig_req_pdu_info_pdupadding,
	&ett_fapi_dlconfig_req_pdu_info_pduunion,

	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dciformat,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_cceIndex,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_rnti,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu,

	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_aggrlvl,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_resallocationtype,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_mcs,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rv,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rbcoding,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_ndi,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_harqproc,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_tpc,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_dai,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_txpower,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_rntitype,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1_padding,

	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_aggrlvl,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_vrbassignment,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_mcs,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rv,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rbcoding,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_ndi,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_harqproc,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_tpc,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_dai,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_allocprach,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_preambleidx,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_txpower,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_prachmaskidx,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_rntitype,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_dcipdu_dcipdu_1a_padding,

	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pdulen,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pduidx,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rnti,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_resallocationtype,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_vrbassignment,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rbcoding,
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_mcs,							 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_rv,							 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transportblocks,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_tb2cwswapflag,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_transmissionscheme,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numlayers,						 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numsubbands,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_uecategory,						 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_pa,							 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_deltapoweroffsetaindex,				 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_ngap,						 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_nprb,						 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numrbpersubband,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_numbfVector,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_subbandInfo,					 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo,				 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_subbandidx,			 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_numantenna,			 
  	&ett_fapi_dlconfig_req_pdu_info_pduunion_dlschpdu_beamformingvectorinfo_bfvalueperantenna,
	
	&ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pdulen,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_pduidx,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_txpower,
	&ett_fapi_dlconfig_req_pdu_info_pduunion_bchpdu_padding,

	&ett_fapi_ulconfig_req,
	&ett_fapi_ulconfig_req_sfnsf,
	&ett_fapi_ulconfig_req_sfnsf_sfn,
	&ett_fapi_ulconfig_req_sfnsf_sf,
	&ett_fapi_ulconfig_req_ulconfiglen,
	&ett_fapi_ulconfig_req_numpdu,
	&ett_fapi_ulconfig_req_rachfreqresources,
	&ett_fapi_ulconfig_req_rachfreqresources_fdd,
	&ett_fapi_ulconfig_req_srs_present,
	&ett_fapi_ulconfig_req_padding,
	&ett_fapi_ulconfig_req_pdu_info,
	&ett_fapi_ulconfig_req_pdu_info_pdutype,
	&ett_fapi_ulconfig_req_pdu_info_pdusize,
	&ett_fapi_ulconfig_req_pdu_info_padding,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo,

	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_handle,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_rnti,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_length,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_dataoffset,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_timingadvance,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ulcqi,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_ri,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_cqipdu_padding,

	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_handle,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_rnti,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_padding,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_pucchindex1,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_harqsize,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_harqpdu_harqinfo_padding,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_handle,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_rnti,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_harqsize,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_pucchindex1,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_harqinfo_padding,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srharqpdu_srinfo_pucchindex,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_handle,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_rnti,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_srpdu_srinfo_pucchindex,

	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_handle,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_size,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rnti,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rbstart,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_numofrb,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_modulationtype,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_cyclicshift2fordmrs,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingenabled,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_freqhoppingbits,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_ndi,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_rv,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_harqproc,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_txmode,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_currtxnb,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschpdu_nsrs,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_handle,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_size,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rnti,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rbstart,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_numofrb,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_modulationtype,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_cyclicshift2fordmrs,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingenabled,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_freqhoppingbits,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_ndi,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_rv,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_harqproc,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_txmode,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_currtxnb,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_ulschpduinfo_nsrs,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_harqsize,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_deltaoffsetharq,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_acknackmode,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_harqinfo_padding,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_nsrsinitial,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_initialnumrb,
	&ett_fapi_ulconfig_req_pdu_info_pduconfiginfo_ulschharqpdu_initialtxparaminfo_padding,

	&ett_fapi_dldatatx_req,
	&ett_fapi_dldatatx_req_sfnsf,
	&ett_fapi_dldatatx_req_sfnsf_sfn,
	&ett_fapi_dldatatx_req_sfnsf_sf,
	&ett_fapi_dldatatx_req_numofpdu,
	&ett_fapi_dldatatx_req_dlpdu_info,
	&ett_fapi_dldatatx_req_dlpdu_info_pdulen,
	&ett_fapi_dldatatx_req_dlpdu_info_pduidx,
	&ett_fapi_dldatatx_req_dlpdu_info_numoftlv,
	&ett_fapi_dldatatx_req_dlpdu_info_tlvinfo,
	&ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_tag,
	&ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_taglen,
	&ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_padding,
	&ett_fapi_dldatatx_req_dlpdu_info_tlvinfo_value,

	&ett_fapi_rxulsch_ind,
	&ett_fapi_rxulsch_ind_sfnsf,
	&ett_fapi_rxulsch_ind_sfnsf_sfn,
	&ett_fapi_rxulsch_ind_sfnsf_sf,
	&ett_fapi_rxulsch_ind_numofpdu,
	&ett_fapi_rxulsch_ind_datapduinfo,
	&ett_fapi_rxulsch_ind_pdubuffer,
	&ett_fapi_rxulsch_ind_datapduinfo_handle,
	&ett_fapi_rxulsch_ind_datapduinfo_rnti,
	&ett_fapi_rxulsch_ind_datapduinfo_length,
	&ett_fapi_rxulsch_ind_datapduinfo_dataoffset,
	&ett_fapi_rxulsch_ind_datapduinfo_timingadvance,
	&ett_fapi_rxulsch_ind_datapduinfo_ulcqi,
	&ett_fapi_rxulsch_ind_datapduinfo_padding,
	&ett_fapi_rxulsch_ind_datapduinfo_data,

	&ett_fapi_ulcrc_ind,
	&ett_fapi_ulcrc_ind_sfnsf,
	&ett_fapi_ulcrc_ind_sfnsf_sfn,
	&ett_fapi_ulcrc_ind_sfnsf_sf,
	&ett_fapi_ulcrc_ind_numofcrc,
	&ett_fapi_ulcrc_ind_crcpdu,
	&ett_fapi_ulcrc_ind_crcpdu_handle,
	&ett_fapi_ulcrc_ind_crcpdu_rnti,
	&ett_fapi_ulcrc_ind_crcpdu_crcflag,
	&ett_fapi_ulcrc_ind_crcpdu_padding,

  	&ett_fapi_hidci0_ind,
  	&ett_fapi_hidci0_sfnsf,
  	&ett_fapi_hidci0_sfnsf_sfn,
  	&ett_fapi_hidci0_sfnsf_sf,
  	&ett_fapi_hidci0_numofdci,
 	&ett_fapi_hidci0_numofhi,										
  	&ett_fapi_hidci0_hidci0pdu_info,									
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu,
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu_pdutype,
 	&ett_fapi_hidci0_hidci0pdu_info_hipdu_hipdusize,							
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu_rbstart,						
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu_cyclicshift2fordmrs,
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu_hivalue,							
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu_iphich,								
  	&ett_fapi_hidci0_hidci0pdu_info_hipdu_txpower,							
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0,							
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_pdutype,							
 	&ett_fapi_hidci0_hidci0pdu_info_hidci0_dcipdusize,							
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_dciformat,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_cceindex,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_rnti,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_agglevel,							
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_rbstart,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_numofrb,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_cyclicshift2fordmrs,
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_mcs,					
 	&ett_fapi_hidci0_hidci0pdu_info_hidci0_freqenabledflag,
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_freqhoppingbits,					
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_newdataindication,
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_uetxantennaselection,
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_tpc,		
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_cqirequest,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_ulindex,						
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_dlassignmentindex,
 	&ett_fapi_hidci0_hidci0pdu_info_hidci0_padding,					
  	&ett_fapi_hidci0_hidci0pdu_info_hidci0_tpcbitmap,

	&ett_fapi_rach_ind,
	&ett_fapi_rach_ind_sfnsf,
	&ett_fapi_rach_ind_sfnsf_sfn,
	&ett_fapi_rach_ind_sfnsf_sf,
	&ett_fapi_rach_ind_numofpreambles,
	&ett_fapi_rach_ind_rachpdu_info,
	&ett_fapi_rach_ind_rachpdu_info_rnti,
	&ett_fapi_rach_ind_rachpdu_info_timingadvance,
	&ett_fapi_rach_ind_rachpdu_info_preamble,
	&ett_fapi_rach_ind_rachpdu_info_padding,

	&ett_fapi_ulharq_ind,
	&ett_fapi_ulharq_ind_sfnsf,
	&ett_fapi_ulharq_ind_sfnsf_sfn,
	&ett_fapi_ulharq_ind_sfnsf_sf,
	&ett_fapi_ulharq_ind_numofharq,
	&ett_fapi_ulharq_ind_harqpdu_info,
	&ett_fapi_ulharq_ind_harqpdu_info_handle,
	&ett_fapi_ulharq_ind_harqpdu_info_rnti,
	&ett_fapi_ulharq_ind_harqpdu_info_harqtb1,
	&ett_fapi_ulharq_ind_harqpdu_info_harqtb2,
    };

    proto_fapi = proto_register_protocol ("FAPI", "FAPI", "fapi");

    proto_register_field_array(proto_fapi, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));

    register_dissector("fapi", dissect_fapi, proto_fapi);
}

void proto_reg_handoff_fapi(void)
{
    static dissector_handle_t fapi_handle;

    fapi_handle = create_dissector_handle(dissect_fapi, proto_fapi);

    dissector_add_uint("udp.port", FAPI_PORT_RD, fapi_handle);
    dissector_add_uint("udp.port", FAPI_PORT_WR, fapi_handle);
}
