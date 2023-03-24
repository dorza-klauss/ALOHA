#ifndef PACKET_HPP
#define PACKET_HPP

class packet {
    int id;
    char type;
    char status;

    double scheduled_time;
    double first_scheduled_time;
    double first_send_attempt;
    double sent_time;
    double expected_receive_time;
    double receive_time;

    int retransmission_attempts_;

    int from_;
    int to_;
    int payload_;

    public:

        //pseudo constants

        static char MSG()           {return 0x00; }; /*0000 0000*/
        static char ACK()           {return 0x10; }; /*0001 0000*/
        static char UNDEFINED()     {return 0x20; }; /*0010 0000*/
        static char SENDING()       {return 0x30; }; /*0011 0000*/
        static char RECEIVED()      {return 0x40; }; /*0100 0000*/
        static char SCHEDULED()     {return 0x50; }; /*0101 0000*/
        static char ACKNOWLEDGED()  {return 0x20; }; /*0110 0000*/


        packet():
        
            id(-1),
            type(packet::UNDEFINED()),
            status(packet::UNDEFINED()),
            scheduled_time(-1),
            first_scheduled_time(-1),
            first_send_attempt(-1),
            sent_time(-1),
            expected_receive_time(-1),
            receive_time(-1),
            retransmission_attempts_(0),
            from_(-1),
            to_(-1),
            payload_(-1){};


        //getters and setters

        void set_id(int i){ id = i;}
        int get_id() const {return id;}

        void set_type(char c){type = c;}
        char get_type() const {return type;}

        void set_status(char c){status=c;}
        char get_status() const {return status;}

        void set_scheduled_time(double t){scheduled_time = t;}
        double get_scheduled_time() const {return scheduled_time;}

        void set_first_scheduled_time(double t){first_scheduled_time = t;}
        double get_first_scheduled_time() const {return first_scheduled_time;}

        void set_expected_receive_time(double t){expected_receive_time = t;}
        double get_expected_recieve_time() const {return expected_receive_time;}

        void set_to (int i) {to_ = i;}
        int get_to() const {return to_;}

        void set_from (int i) {from_=i;}
        int get_from() const {return from_;}

        void set_payload(int i) {payload_ = i;}
        int get_payload() const {return payload_;}

        //Method Declaration

        void sent(double time);
        void received(double time);
        void output() const;
        double get_effective_transmission_time() const;
        double get_total_transmission_time() const;
        double get_total_delay() const;

};

#endif
