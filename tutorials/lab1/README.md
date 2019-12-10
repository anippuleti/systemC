Generic FIFO implementation is discussed in this Lab. API is provided for user
to push and pop elements into FIFO. Reference to SystemC events {Full, Empty,
almost-full and almost-empty} is provided to notify the user code.

Two Tests in tb_fifo.cpp instantiate FIFO and run traffic. Test1 is basic test.
Test2 shows how the FIFO can run at different Read/Write clock frequencies.
Implementation takes care of race between Read and write to same location.
User code must make sure that there exists atleast 1 element in FIFO
before invoking method 'get_element()'.

Implementation shows how various HW components can be implemented at higher
level of abstraction (without input/output ports, clocks) for simulation 
speed while not loosing the accuracy. Also shows how to build generic HW
SystemC modules with the help of C++ templates for re-usability. 

Refer to Documentation for additional details.
