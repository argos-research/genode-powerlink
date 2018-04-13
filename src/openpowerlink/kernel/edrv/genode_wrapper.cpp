#include "genode_wrapper.h"

#ifdef __cplusplus

/* 
* Implementation according to /lib/lwip/platform/nic.cc file written by:
* Stefan Kalkowski (date: 2009-11-05)
* as part of the LwIP ethernet interface for Genode (developed by Genode Labs GmbH)
*/

#include <base/thread.h>
#include <base/log.h>
#include <nic/packet_allocator.h>
#include <nic_session/connection.h>

#define PACKET_SIZE 	1536;

 /*
 * Thread, that receives packets by nic-session
 */
class Nic_receiver_thread : public Genode::Thread_deprecated<8192>
{
	private:

		typedef Nic::Packet_descriptor Packet_descriptor;

		Nic::Connection  *_nic;       /* nic-session */
		Packet_descriptor _rx_packet; /* actual packet received */
		tEdrvInstance *_init;	      /* ethernet driver instance of openPowerlink */

		Genode::Signal_receiver  _sig_rec;

		Genode::Signal_dispatcher<Nic_receiver_thread> _rx_packet_avail_dispatcher;
		Genode::Signal_dispatcher<Nic_receiver_thread> _rx_ready_to_ack_dispatcher;

		void _handle_rx_packet_avail(unsigned)
		{
			while (_nic->rx()->packet_avail() && _nic->rx()->ready_to_ack()) {
				_rx_packet = _nic->rx()->get_packet();
				process_input(_init);
				_nic->rx()->acknowledge_packet(_rx_packet);
			}
		}

		void _handle_rx_read_to_ack(unsigned) { _handle_rx_packet_avail(0); }

		void _tx_ack(bool block = false)
		{
			/* check for acknowledgements */
			while (nic()->tx()->ack_avail() || block) {
				Packet_descriptor acked_packet = nic()->tx()->get_acked_packet();
				nic()->tx()->release_packet(acked_packet);
				block = false;
			}
		}

	public:

		Nic_receiver_thread(Nic::Connection *nic, tEdrvInstance *init)
		:
			Genode::Thread_deprecated<8192>("nic-recv"), _nic(nic), _init(init),
			_rx_packet_avail_dispatcher(_sig_rec, *this, &Nic_receiver_thread::_handle_rx_packet_avail),
			_rx_ready_to_ack_dispatcher(_sig_rec, *this, &Nic_receiver_thread::_handle_rx_read_to_ack)
		{
			_nic->rx_channel()->sigh_packet_avail(_rx_packet_avail_dispatcher);
			_nic->rx_channel()->sigh_ready_to_ack(_rx_ready_to_ack_dispatcher);
		}

		void entry();
		Nic::Connection  *nic() { return _nic; };
		Packet_descriptor rx_packet() { return _rx_packet; };

		void sendTXBufferWorkerThread(unsigned char* buffer, size_t size) {
			int i = 0;
			char out[((int)size)+1]; //null terminator
			for (i = 0; i < ((int)size)/3; i++) {
			    snprintf(out+i*3, 4, "%02x ", buffer[i]);
			}
			Genode::log((const char *)out);
	  		Nic::Packet_descriptor packet;
	  		bool end = FALSE;

	  		while (!end) {
	            try {
	                packet = nic()->tx()->alloc_packet(size);
	                end = TRUE;
	            } catch(Nic::Session::Tx::Source::Packet_alloc_failed) {
	                /* packet allocator exhausted, wait for acknowledgements */
	                Genode::log("Too many packets");
	                _tx_ack(true);
	            }
	        }

	        char *tx_content = nic()->tx()->packet_content(packet);

			/*
			 * copy payload into packet's payload
			 */
			Genode::memcpy(tx_content, buffer, size);

			/* Submit packet */
			nic()->tx()->submit_packet(packet);
			/* check for acknowledgements */
			_tx_ack(false);
	    }

	    	static void	process_input(tEdrvInstance *init)
		{
			tEdrvRxBuffer           rxBuffer;

			Nic_receiver_thread   *th         = reinterpret_cast<Nic_receiver_thread*>(init->genodeEthThread);
			Nic::Connection       *nic        = th->nic();
			Nic::Packet_descriptor rx_packet  = th->rx_packet();
			char                  *rx_content = nic->rx()->packet_content(rx_packet);
			//u16_t                  len        = rx_packet.size();

			rxBuffer.bufferInFrame = kEdrvBufferMiddleInFrame;

		        // Get length of received packet
		        // size does not contain CRC as RW_REGW_CPLUS_COMMAND_RX_CHKSUM_OFLD_EN is set
		        rxBuffer.rxFrameSize = (rx_packet.size()) - EDRV_ETH_CRC_SIZE;
			
		        rxBuffer.pBuffer = (UINT8*)rx_content; 
			//Genode::memcpy(rxBuffer.pBuffer, rx_content, rx_packet.size());

                    	// Call Rx handler of Data link layer
                    	init->initParam.pfnRxHandler(&rxBuffer);
		}
};

/*
 * C-interface
 */
extern "C" {
#endif
	using namespace Genode;
	using namespace Net;

	Nic::Connection  *_nic;       /* nic-session */

  	Nic::Connection  *nic() { return _nic; };

	int init_Session(tEdrvInstance *init) {
	    Genode::log("Open NIC Session");
		/* Initialize nic-session */
		Nic::Packet_allocator *tx_block_alloc = new (env()->heap())Nic::Packet_allocator(env()->heap());

		int buf_size    = Nic::Session::QUEUE_SIZE * PACKET_SIZE;

		try {
			_nic = new (env()->heap()) Nic::Connection(tx_block_alloc,
			                                          buf_size,
			                                          buf_size);
		} catch (Parent::Service_denied) {
			destroy(env()->heap(), tx_block_alloc);
			return 1;
		}

		Nic_receiver_thread *th = new (env()->heap())Nic_receiver_thread(nic(), init);

		init->genodeEthThread = (void*) th;

		th->start();

		return 0;
  	}

  	void get_Mac_Address(UINT8 addr[6]) {
    	//Get Mac Address from Genode NIC
    	Nic::Mac_address _mac_address = nic()->mac_address();

    	for(int i=0; i<6; ++i)
			addr[i] = _mac_address.addr[i];

		Genode::log(_mac_address);
  	}


  	void sendTXBuffer(tEdrvInstance *init, unsigned char* buffer, size_t size) {
  		Nic_receiver_thread *th = reinterpret_cast<Nic_receiver_thread*>(init->genodeEthThread);

		th->sendTXBufferWorkerThread(buffer, size);
    }
    




#ifdef __cplusplus
} //end extern "C"
#endif

void Nic_receiver_thread::entry()
		{
			Genode::log("NIC Thread entry() started");
			while(true)
			{
				Genode::Signal sig = _sig_rec.wait_for_signal();
				int num    = sig.num();

				Genode::Signal_dispatcher_base *dispatcher;
				dispatcher = dynamic_cast<Genode::Signal_dispatcher_base *>(sig.context());
				dispatcher->dispatch(num);
			}
		}
