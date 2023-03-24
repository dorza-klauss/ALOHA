using namespace std;

#include "packet.hpp"
#include "wirelessnode.hpp"

#undef SLOTTED_ALOHA


vector<Wireless_node> list_nodes;
vector<packet> buffer;
vector<packet> buffer_received;
double current_time = 0;

#ifdef SLOTTED_ALOHA

double next_slot = 0;
double slot_intervals = 0.02;

#endif

void pause(){
    std::string sInputString;
    //wait for input from stdin/keyboard
    cout<<"Type anything to continue...";
    std::cin>>sInputString;
}

void init(double rate, double trans_time){
    list_nodes.clear();
    buffer.clear();
    buffer_received.clear();
    current_time = 0;

    for(int i=0;i<6;i++){
        Wireless_node node;
		node.set_id(i);
		switch (i)
		{
			case 0:
				node.set_seed(4.49834e+008);
				break;
			case 1:
				node.set_seed(7.51391e+008);
				break;
			case 2:
				node.set_seed(5.648e+008);
				break;
			case 3:
				node.set_seed(1.42815e+009);
				break;
			case 4:
				node.set_seed(1.48530e+009);
				break;
			case 5:
				node.set_seed(1.77402e+009);
				break;
		}

        node.set_transmission_rate(rate);
        node.set_propagation_time(trans_time);
        
        #ifdef SLOTTED_ALOHA
            node.set_eff_factor(-1);
        #else
            node.set_eff_factor(-2);
        #endif

            node.initialize();
            list_nodes.push_back(node);
		
    }

    //put messages in buffer

    buffer.push_back(list_nodes[0].create_new_message_packet(1,current_time));
    buffer.push_back(list_nodes[2].create_new_message_packet(3,current_time));
    buffer.push_back(list_nodes[4].create_new_message_packet(5,current_time));
    
    #ifdef SLOTTED_ALOHA
        next_slot += slot_intervals;
    #endif
}

void simulate(double rate, double trans_time){
    bool ready_to_stop = false;

    init(rate,trans_time);
    while(!ready_to_stop){

        /* Update current_time
         * loop through buffer and see which is first, new packets to send or existing packets to receive
        */

        double next_time = 1000000;
        packet *pkt = NULL;
        Wireless_node *node = NULL;
        int pkt_idx = -1;

        for(int i=0;i<buffer.size();i++){

            if(
                (buffer[i].get_status()==packet::SCHEDULED() && buffer[i].get_scheduled_time() < next_time)
                ||
                (buffer[i].get_status()==packet::SENDING() && buffer[i].get_expected_recieve_time()<next_time)
                ){
                    pkt = &buffer[i];
                    node = &list_nodes[pkt->get_from()];
                    if (pkt->get_status()==packet::SCHEDULED()) next_time = pkt->get_scheduled_time();
                    if(pkt->get_status()==packet::SENDING()) next_time= pkt->get_expected_recieve_time();
                    pkt_idx = i;
                }
        }

        current_time = next_time;

        if(pkt->get_status()==packet::SENDING()){
            node->completed_transmission(pkt,current_time);
            //add to completed packets buffer
            buffer_received.push_back(buffer[pkt_idx]);

            if (pkt->get_type()==packet::MSG()){
                buffer.push_back(list_nodes[pkt->get_to()].create_new_ack_packet(pkt->get_from(),current_time,pkt->get_id()));
            }
            else{

                //received acknowledgement
                for(int i=0;i<buffer_received.size();i++){

                    if(buffer_received[i].get_type()==packet::MSG()
                        &&pkt->get_to() == buffer_received[i].get_from()
                        &&pkt->get_from() == buffer_received[i].get_to()
                        &&pkt->get_payload() == buffer_received[i].get_id())
                        {
                            if (buffer_received[i].get_status() != packet::ACKNOWLEDGED()){
                                buffer_received[i].set_status(packet::ACKNOWLEDGED());
                                buffer.push_back(list_nodes[pkt->get_to()].create_new_message_packet(pkt->get_from(), current_time));

                            }
                        }
                        

                    }
                }

            //removing the completed message from buffer to speed up the search process

            vector<packet>::iterator startIterator;
            for(startIterator = buffer.begin(); startIterator!=buffer.end();startIterator++){
                if((((packet)*startIterator).get_id()==buffer[pkt_idx].get_id()) && (((packet)*startIterator).get_from())==buffer[pkt_idx].get_from()){
                    buffer.erase(startIterator);
                    break;
                }
            }
        }
        else{

            if (pkt->get_status()==packet::SCHEDULED()){

#ifdef SLOTTED_ALOHA

                //update time to next time slot
                if(current_time<next_slot){
                    node->reschedule_packet(pkt,next_slot);
                }

                else{
                    next_slot +=slot_intervals;
#endif
                    //check for collisions
                    bool collisions = false;
                    for(int i=0;i<buffer.size();i++){
                        if(
                            (buffer[i].get_status()==packet::SENDING() &&buffer[i].get_expected_recieve_time()>current_time)
                            ||
                            (buffer[i].get_status()==packet::SCHEDULED() && buffer[i].get_scheduled_time()==current_time && buffer[i].get_from() != pkt->get_from())

                        ){
                            packet *pkt_sending = &buffer[i];
                            Wireless_node *node_sending = &list_nodes[pkt_sending->get_from()];
                            node_sending->collision(pkt_sending,current_time);
                            collisions = true;
                        }

                    }

                    node->attempt_transmission(pkt,current_time); //since it was scheduled, let's transmit (attempt)
#ifdef SLOTTED_ALOHA
                }
#endif

            }



        }

        //test if simulation should stop
        bool test = true;
        for(int i=0; i<list_nodes.size();i++){
            test &= list_nodes[i].is_within_error_range(0.06);
        }

        ready_to_stop = test;


    }


    cout<<"Total Simulation Time: "<<current_time<<endl;
    double t = 0.0, a = 0.0, th = 0.0, d = 0.0;

    for(int i =0; i<list_nodes.size(); i++){
        Wireless_node *n = &list_nodes[i];
        t+= n->get_completed_transmissions();
        a+= n->get_attempted_transmissions();
        th+= n->get_efficiency();
        d+= n->get_average_delay();
    }

    cout << "[Completed transmission] " << t/list_nodes.size() <<" [Number of transmission attempts] "<<a/list_nodes.size()
        << " [Throughput] "<<th/list_nodes.size() << " [G] "<<rate*trans_time<<" [Average Delay] "<<d/list_nodes.size()<<endl;
}


int main(int argc, char* _argv[]){
    for(int i =200; i >30; i = i -10){
        simulate(i,0.01);
    }

    return (0);
}


