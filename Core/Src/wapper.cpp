#include <main.h>
#include <CRSLibtmp/std_type.hpp>
#include <CRSLibtmp/Can/Stm32/RM0008/can_bus.hpp>
#include <CRSLibtmp/Can/Stm32/RM0008/filter_manager.hpp>

using namespace CRSLib::IntegerTypes;
using namespace CRSLib::Can::Stm32::RM0008;
using CRSLib::Can::DataField;

extern "C" void mymain(CAN_HandleTypeDef *const hcan)
{
	// ここでCANのMSP(ピンやクロックなど。ここまで書ききるのはキツかった...)の初期化を行う
	HAL_CAN_DeInit(hcan);
	HAL_CAN_MspInit(hcan);

	enum FilterName : u8
	{
//		Servo,
//		InjectSpeed,
//		SolenoidValve,
//		MotorState,
		GreetFromRos,
	};

//	FilterConfig filter_configs[4];
//	filter_configs[Servo] = FilterConfig::make_default(Fifo::Fifo0);
//	filter_configs[InjectSpeed] = FilterConfig::make_default(Fifo::Fifo0);
//	filter_configs[SolenoidValve] = FilterConfig::make_default(Fifo::Fifo0);
//	filter_configs[MotorState] = FilterConfig::make_default(Fifo::Fifo1);

	FilterConfig filter_configs[1];
	filter_configs[GreetFromRos] = FilterConfig::make_default(Fifo::Fifo0);
	
	// *先に*フィルタの初期化を行う。先にCanBusを初期化すると先にNormalModeに以降してしまい、これはRM0008に違反する。
	FilterManager::initialize(filter_bank_size, filter_configs);

	// 通信開始
	CanBus can_bus{can1};

	while(true)
	{
		const auto message = can_bus.receive(Fifo::Fifo0);
		if(message && message->data.dlc >= 3) (void)can_bus.post(0x300, DataField{.buffer={(byte)'H', (byte)'E', (byte)'L', (byte)'L', (byte)'O', message->data.buffer[0], message->data.buffer[1], message->data.buffer[2]}, .dlc=8});
	}
}
