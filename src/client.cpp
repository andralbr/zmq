#include <iostream>
#include <string>

#include <zmq.hpp>


/// Print vector
template< typename T >
void printVec( T* vec, int size )
{
    for (int cc = 0; cc < (size-1); cc++ )
    {
        std::cout << vec[cc] << ", ";
    }
    std::cout << vec[size-1] << "\n";
}


int main()
{

    // Create socket (request type)
    zmq::context_t ctx;
    zmq::socket_t socket(ctx, zmq::socket_type::req );    // = ZMQ_REQ

    // Connect to socket
    socket.connect("tcp://127.0.0.1:5580");

    // Send message
    socket.send(zmq::buffer("DATA REQUEST"), zmq::send_flags::none ); 

    // Receive message via buffer
    // ---------------------------
    // If less memory associated, data will be dropped.
    // More memory is ok, but no info transmitted about actual data size in memory.

    /*
    double* vec1 = new double[5]; 
    zmq::mutable_buffer buffer(static_cast<void*>(vec1), 5*sizeof(double));

    socket.recv( buffer, zmq::recv_flags::none );
    std::cout << "Received message of size = " << buffer.size() << "\n";
    std::cout << "vec1 = ";
    printVec<double>(vec1, 5);
    */

    // or ...
    std::vector<double> vec1(5);
    zmq::mutable_buffer buffer( (void*) &vec1[0], vec1.size() *sizeof(double));

    socket.recv( buffer, zmq::recv_flags::none );
    std::cout << "Received message of size = " << buffer.size() << "\n";
    std::cout << "vec1 = " << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << ", " << vec1[3] << ", " << vec1[4] << "\n";


    // Receive message via message_t
    // -----------------------------    
    zmq::message_t msg;
    int countMsg = 2;
    while( true )
    {
        // Receive message
        socket.recv(msg, zmq::recv_flags::none );
        std::cout << "Received message " << countMsg << " of size " << msg.size() << "\n";
        
        if ( countMsg == 2)
        {
            // This is the float array
            float* vec2 = msg.data<float>();
            int vec2_size = msg.size()/sizeof(float);
            std::cout << "vec2 = ";
            printVec<float>(vec2, vec2_size);
        }

        if ( countMsg == 3)
        {
            // This is the integer array
            int* vec3 = msg.data<int>();
            int vec3_size = msg.size() / sizeof(int);
            std::cout << "vec3 = ";
            printVec<int>(vec3, vec3_size);   
        }

        if ( countMsg == 4)
        {
            // This is the float array
            double* vec4 = msg.data<double>();
            int vec4_size = msg.size()/sizeof(double);
            std::cout << "vec4 = ";
            printVec<double>(vec4, vec4_size);
        }

        if ( countMsg == 5)
        {
            // This is a double array
            double* vec5 = msg.data<double>();
            int vec5_size = msg.size()/sizeof(double);
            std::cout << "vec5 = ";
            printVec<double>(vec5, vec5_size);
        }

        if ( countMsg == 6)
        {
            // This is a string message
            std::cout << msg.to_string() << "\n";
        }
        // If not more messages => stop receiving
        if (! msg.more() )
            break;

        // Increase message counter and empy "msg"
        countMsg++;
        msg.rebuild();
    }


    /* ---------------------------------------------------
     * Use:
     *  - buffer: for import in existing data structures
     *      (e.g. struct element) & when properties (e.g. size, 
     *       how many multi-messages) are known.
     *  - message_t: for all other cases, when there is no
     *      memory allocated yet.
     * ----------------------------------------------------*/
    


    return 0;
}