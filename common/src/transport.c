#include "transport.h"

/**
 * @brief Initializes the transport interface with the specified type and configuration.
 * @param transport Pointer to the Transport structure.
 * @param type The transport type to initialize.
 * @param config Pointer to transport specific configuration structure.
 * @return 0 on success, -1 on failure or invalid transport type.
 */
int transport_init(Transport_t* transport, TransportType_t type, void* config) {
    transport->type = type;
    transport->config = config;
    
    // Set function pointers based on type
    switch (type) {
        case TRANSPORT_UART:
            transport->init = uart_transport_init;
            transport->send = uart_transport_send;
            transport->receive = uart_transport_receive;
            transport->process = uart_transport_process;
            transport->deinit = uart_transport_deinit;
            break;
        default:
            return -1; // Invalid transport type
    }
    
    // Initialize the selected transport
    if (transport->init(config) != 0) {
        return -1;
    }
    
    transport->initialized = 1;
    return 0;
}


/**
 * @brief Sends data using the initialized transport.
 * @param transport Pointer to the initialized Transport structure.
 * @param data Pointer to the data buffer to send.
 * @param len Length of the data buffer.
 * @return Number of bytes sent on success, -1 if transport is not initialized or on error.
 */
int transport_send(Transport_t* transport, const uint8_t* data, size_t len) {
    if (!transport->initialized) {
        return -1;
    }
    
    return transport->send(data, len);
}


/**
 * @brief Receives data using the initialized transport.
 * @param transport Pointer to the initialized Transport structure.
 * @param data Pointer to the buffer to store received data.
 * @param len Maximum number of bytes to receive.
 * @return Number of bytes received on success, -1 if transport is not initialized or on error.
 */
int transport_receive(Transport_t* transport, uint8_t* data, size_t len) {
    if (!transport->initialized) {
        return -1;
    }
    
    return transport->receive(data, len);
}


/**
 * @brief Processes pending events or tasks related to the transport.
 * @param transport Pointer to the initialized Transport structure.
 * @return 0 on success, -1 if transport is not initialized or on error.
 */
int transport_process(Transport_t* transport) {
    if (!transport->initialized) {
        return -1;
    }
    
    return transport->process();
}

/**
 * @brief Deinitializes the transport interface.
 * @param transport Pointer to the initialized Transport structure.
 * @return 0 on success, -1 if transport is not initialized or on error.
 */
int transport_deinit(Transport_t* transport) {
    if (!transport->initialized) {
        return -1;
    }
    
    int result = transport->deinit();
    if (result == 0) {
        transport->initialized = 0;
    }
    
    return result;
}