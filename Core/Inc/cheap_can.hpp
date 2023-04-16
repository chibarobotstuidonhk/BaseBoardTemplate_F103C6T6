#pragma once

#include <variant>

#include <CRSLibtmp/std_type.hpp>
#include <CRSLibtmp/bit_operation.hpp>
#include <CRSLibtmp/Can/utility.hpp>
#include <CRSLibtmp/Can/Stm32/RM0008/register_map.hpp>

namespace Can
{
	using namespace CRSLib::Can::Stm32::RM0008;
	using namespace RegisterMap;
}

namespace Ijector
{
	using namespace CRSLib::IntegerTypes;
	using namespace CRSLib::BitOperation;

	class CheapCan final
	{
		enum class State
		{
			SleepMode,
			InitializationMode,
			NomalMode,
			Transient
		};

		Can::CanRegister *const bxcan;
		State state{State::SleepMode};

		CheapCan(Can::CanRegister *const bxcan) noexcept:
			bxcan{bxcan}
		{}

		private:
		void leave_sleep() noexcept
		{
			clear_bit(bxcan->MCR, Can::MCR::SLEEP);
			while(is_bit_clear(bxcan->MSR, Can::MSR::SLAK));
		}

		void enter_sleep() noexcept
		{
			set_bit(bxcan->MCR, Can::MCR::SLEEP);
			while(is_bit_set(bxcan->MSR, Can::MSR::SLAK));
		}

		void enter_initialization() noexcept
		{
			set_bit(bxcan->MCR, Can::MCR::INRQ);
			while(is_bit_set(bxcan->MSR, Can::MSR::INAK));
		}

		void leave_initialization() noexcept
		{
			clear_bit(bxcan->MCR, Can::MCR::INRQ);
			while(is_bit_clear(bxcan->MSR, Can::MSR::INAK));
		}



		// static auto default_can_handle(const CRSLib::Can::Bitrate bitrate = CRSLib::Can::Bitrate::rate1M) noexcept -> CAN_HandleTypeDef
		// {
		// 	return CAN_HandleTypeDef
		// 	{
		// 		// レジスタのポインタ
		// 		.Instance = CAN1,

		// 		// HAL_CAN_Initするためだけに使われる. それ以外じゃ使われない. なぜ他と混ぜてしまったのかは不明.
		// 		// ほぼCanClass2(Ryunika先輩作)からコピペしたもの.
		// 		// Prescalerは多分4であってるんじゃないかな...ここだけ実行時に決定するように書かれてたので変更
		// 		.Init =
		// 		CAN_InitTypeDef
		// 		{
		// 			// RM0008のFig. 234あたり参照. APB1が36ならばこれでよい.
		// 			.Prescaler = HAL_RCC_GetPCLK1Freq() / 18 / CRSLib::to_underlying(bitrate),

		// 			// Test Modeに変更することもできる.
		// 			.Mode = CAN_MODE_NORMAL,

		// 			// RM0008のFig. 234あたり参照. APB1が36ならばこれでよい.
		// 			//Sample-Point at: (1+15)/(1+15+2)=88.9% where CAN open states "The location of the sample point must be as close as possible to 87,5 % of the bit time."
		// 			.SyncJumpWidth = CAN_SJW_1TQ,
		// 			.TimeSeg1 = CAN_BS1_15TQ,
		// 			.TimeSeg2 = CAN_BS2_2TQ,

		// 			// datafield8byteの後ろ2byteを時間の送信に割くか
		// 			// 使う場合にはdlcは常に8
		// 			.TimeTriggeredMode = DISABLE,
		// 			// このモード設定に関係なくTECが多くなると自動でBus-Offする. このモードがオンだと自動で復帰する.
		// 			.AutoBusOff = ENABLE,
		// 			// CAN bus activityが検知されると自動でSleep Modeを抜けるかどうか. よくわからんのでオフ.
		// 			.AutoWakeUp = DISABLE,
		// 			// Time Triggered Communication用. よくわからん.
		// 			// DISABLE 自動で再送しない. Time Triggered Communicationできるようになるらしい.
		// 			// ENABLE 自動再送する. 普通はこっち.
		// 			.AutoRetransmission = ENABLE,
		// 			// 受信Fifoがあふれたときにどこのメッセージを消すか.
		// 			// ENABLEで新しくきたやつ, DISABLEで一番古いやつ
		// 			.ReceiveFifoLocked = DISABLE,
		// 			// 送信Mailboxに複数個中身があったときの優先順位.
		// 			// ENABLE Mailboxの添え字順
		// 			// DISABLE IDの若さ順
		// 			.TransmitFifoPriority = DISABLE,
		// 		},

		// 		// この2つはグローバルな状態を管理するためのもの. なぜ同じ引数にまとめてしまったのか.

		// 		// HALライブラリの捉え方によるbxCANの状態. 微妙にRM0008系のリファレンスとは状態の捉え方が違う.
		// 		.State = HAL_CAN_STATE_READY,
		// 		// エラーコード.
		// 		.ErrorCode = HAL_CAN_ERROR_NONE
		// 	};
		// }

		void crs_can_init()
		{
			HAL_CAN_DeInit(hcan);
			*hcan = default_can_handle();
			HAL_CAN_Init(hcan);
		}

		void filter_bank_init(CAN_HandleTypeDef *const hcan)
		{

		}
	};
}