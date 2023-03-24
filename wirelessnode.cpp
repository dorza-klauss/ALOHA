#include <cstdlib>
#include "packet.hpp"
#include "wirelessnode.hpp"
using namespace std;

//create a new packet

packet Wireless_node::create_new_packet(char type, int to, double current_time){

    packet pkt;
    pkt.set_id(generated_packets++);
    pkt.set_type(type);
    pkt.set_status(packet::SCHEDULED());
    pkt.set_from(id);
    pkt.set_to(to);
    pkt.set_first_scheduled_time(current_time+rnd_transmission.exponential());
    pkt.set_scheduled_time(pkt.get_first_scheduled_time());
    pkt.set_expected_receive_time(pkt.get_scheduled_time()+propagation_time);
    return pkt;
}

packet Wireless_node::create_new_message_packet(int to, double current_time){
    return create_new_packet(packet::MSG(),to,current_time);
}

packet Wireless_node::create_new_ack_packet(int to, double current_time, int pkt_id){
    packet p = create_new_packet(packet::ACK(), to, current_time);
    p.set_payload(pkt_id);
    return p;
}

//sets the seed of the random variables

void Wireless_node::set_seed(double trans){

    rnd_transmission.set_seed(trans);
}

//creates instances of random variate generator and the first event

void Wireless_node::initialize(){

    if(!initialized){
        rnd_transmission.set_rate(transmission_rate);
        initialized = true;
    }
}

void Wireless_node::output() const{
    cout << "NODE: " << id << " [Completed transmissions]" << completed_transmissions << " [# Transmission attempts]" << attempted_transmissions
			<< " [Throughput]" << efficiency << " [G]" << transmission_rate * propagation_time << " [Average Delay]"  << delay/generated_packets << endl;
}

void Wireless_node::attempt_transmission(packet *p, double current_time){
    attempted_transmissions++;
    p->sent(current_time);
}

void Wireless_node:: completed_transmission(packet *p, double current_time){
    completed_transmissions++;
    p->received(current_time);
    efficiency += p->get_effective_transmission_time()/p->get_total_transmission_time();
    efficiency = efficiency/2;
    delay += p->get_total_delay();
}

void Wireless_node:: reschedule_packet(packet *p, double time){
    p->set_status(packet::SCHEDULED());
    if (p->get_first_scheduled_time() == -1){
        p->set_first_scheduled_time(p->get_scheduled_time());
    }
    p->set_scheduled_time(time);
    p->set_expected_receive_time(p->get_scheduled_time()+propagation_time);

}

void Wireless_node::collision(packet *p, double current_time){
    failed_transmission++;
    reschedule_packet(p, current_time+rnd_transmission.exponential());
}

//returns true if the simulation is within the desired error range
bool Wireless_node:: is_within_error_range(double err) const{
    return (efficiency>0) && (abs(efficiency-get_expected_efficiency())<err);
}

double Wireless_node::get_expected_efficiency() const{
    double G = transmission_rate*propagation_time;
    return G*exp(eff_factor*G);
}
