#ifndef WIRELESSNODE_HPP
#define WIRELESSNODE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#include "packet.hpp"
#include "random_number_generator.hpp"


class Wireless_node{

    int id;

    //These variables are used to evaluate the performance of the node

    int completed_transmissions; //total number of transmissions completed
    int attempted_transmissions; //total number of attempted transmission
    int failed_transmission; //total number of failed transmission;
    int generated_packets; //number of packets generated

    double efficiency;
    double delay;
    int eff_factor;

    //These variables are used for simulation of the node

    Random_Number_Generator rnd_transmission;
    double transmission_rate;
    double propagation_time;
    bool initialized;


    private: 
        packet create_new_packet(char type, int to, double current_time);

    public:
        Wireless_node():
            completed_transmissions(0), attempted_transmissions(0), failed_transmission(0),
            generated_packets(0), efficiency(1), delay(0), eff_factor(1), transmission_rate(100), propagation_time(0.01), initialized(false) {};
        
        //getters and setters

        double get_completed_transmissions() const {
            return completed_transmissions;
        };

        double get_attempted_transmissions() const{
            return attempted_transmissions;
        };
        
        double get_efficiency() const {
            return efficiency;
        }; 

        double get_average_delay() const {
            return delay/generated_packets;
        };

        void set_eff_factor(int i) {
            eff_factor = i;
        };

        void set_id(int i){
            id = i;
        };

        int get_id() const {
            return id;
        };

        void set_transmission_rate(double d) {
            transmission_rate = d;
        };

        double get_transmission_rate() const{
            return transmission_rate;
        };

        void set_propagation_time(double d){
            propagation_time = d;
        };

        double get_propagation_time() const {
            return propagation_time;
        };

        //definition of remaining public functions

        void initialize();
        packet create_new_message_packet(int to, double current_time);
        packet create_new_ack_packet(int to, double current_time, int pkt_id);
        void attempt_transmission(packet *p, double current_time);
        void completed_transmission(packet *p, double current_time);
        void collision(packet *p, double current_time);
        void reschedule_packet(packet *p, double exact_time);
        void set_seed(double transmission);
        void output() const;
        bool is_within_error_range(double err) const;
        double get_expected_efficiency() const;

};


#endif