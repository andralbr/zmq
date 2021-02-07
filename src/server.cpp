#include <iostream>
#include <string>

#include <zmq.hpp>

int main()
{

    // Create a socket
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, zmq::socket_type::rep ); // == ZMQ_REP 

    // Bind to socket [localhost]
    socket.bind("tcp://127.0.0.1:5580");

    // Get message from client
    zmq::message_t msg;
    socket.recv( msg );

    std::cout << "[Server] Received message: " << msg.to_string() << "\n";


    // Send message (no copy)
    std::vector<double> vec1 = {1.0, 2.1, 3.2, 4.3, 5.6}; 
    zmq::mutable_buffer buf1 = zmq::buffer( vec1 );
    vec1[1] = 2.2;    // <- Check: buffer does not create data copy (2.2 send)
    socket.send( buf1, zmq::send_flags::sndmore );

    // ... and another message (no copy)
    std::vector<float> vec2 = {2.5f, 7.3f, 8.1f};
    zmq::mutable_buffer buf2 = zmq::buffer( &vec2[0], vec2.size() * sizeof(float));
    vec2[1] = 5.4f;  // <- Check: buffer does not create data copy (5.4 send)
    socket.send( buf2, zmq::send_flags::sndmore );


    // ... and a message_t (copy)
    std::vector<int> vec3 = {4, 5, 6, 7, 8, 9};
    zmq::message_t msg3( vec3.begin(), vec3.end() );
    vec3[1] = 15;  // <- Check: copy created (15 will not be sent)
    socket.send( msg3, zmq::send_flags::sndmore );

    // message_t: initialization with pointer and size (copy)
    std::vector<double> vec4 = {1.0, 2.0, 3.0};
    zmq::message_t msg4( &vec4[0], 3*sizeof(double));
    vec4[1] = 22.0;  // <- Check: copy created (22 not sent)
    socket.send( msg4, zmq::send_flags::sndmore );

    // message_t: No copy syntax (no copy, WARNING: takes ownership)
    /* DO NOT USE: With "freeFct" function memory gets messed up, 
     * w/o (= nullptr) it works for c-array but not for std::vector
     */
    double* vec5 = new double(3);
    vec5[0] = 1.0; vec5[1]=2.0; vec5[2] = 3.0;
    auto freeFct = [](void* data, void* hint ) -> void { free( data ); };
    zmq::message_t msg5( (void*) vec5, 3*sizeof(double), freeFct, nullptr); // or nullptr for function
    vec5[1] = 22.0;   // <- Check, no copy (22 sent)
    socket.send( msg5, zmq::send_flags::sndmore );

    // ... a final good-bye
    zmq::message_t msg7("This is it!", 11);
    socket.send( msg7, zmq::send_flags::none );

    /* -----------------------------------------------
     * Use ...
     *  - message_t for copied data,
     *  - buffer for exising data not to be copied.
     * -----------------------------------------------*/

    // Protocol Buffers, Boost Endian    

    return 0;
}