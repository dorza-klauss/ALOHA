using namespace std;

#include "packet.hpp"
#include <iostream>

void packet::sent(double time){
    if (first_send_attempt==-1){
        first_send_attempt = time;
    }

    retransmission_attempts_++;
    sent_time = time;
    status = packet::SENDING();
}

void packet::received(double time){
    receive_time = time;
    status = packet::RECEIVED();
}

void packet::output() const{
    cout<< "packet: " << id << " from NODE: "<< from_ <<" [SCH: "<<scheduled_time<<"] "<<"[FIRS: "<<first_send_attempt<<"] "<<"[SENT: "<<sent_time<<"] "
        << "[XP_RCV: "<<expected_receive_time<<"] "<<"[RCV: "<<receive_time<<"] "<<"ATTEMPT: "<<retransmission_attempts_<<" Status: ";
    if(status==SENDING()) cout<<"sending"<<endl;
    if(status==RECEIVED()) cout<<"received"<<endl;
    if(status==SCHEDULED()) cout<<"scheduled"<<endl;
    if(status==ACKNOWLEDGED()) cout<<"acknowledged"<<endl;
}

double packet::get_effective_transmission_time() const{
    return receive_time - sent_time;
}

double packet::get_total_transmission_time() const{
    return receive_time - first_send_attempt;
}

double packet::get_total_delay() const {
    return receive_time - first_scheduled_time;
}

