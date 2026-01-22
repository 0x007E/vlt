/**
 * @file UART_enums.h
 * @brief Enumerations for UART data status, error codes, and handshake states.
 * 
 * This header file defines the enums used to represent UART receive status, communication errors, and hardware/software handshake conditions. It includes enumerations for data reception states, UART frame errors, and flow control signaling.
 * 
 * @note These enums improve code readability by providing meaningful symbolic names for UART-related values, facilitating robust error handling and status reporting in embedded applications.
 * 
 * @author g.raf
 * @date 2025-12-07
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr-common  "AVR Common GitHub Repository"
 */

#ifndef UART_ENUM_H_
#define UART_ENUM_H_

    /**
     * @typedef UART_Data
     * @brief Alias for enum UART_Data_t representing UART receive status.
     */
    typedef enum UART_Data_t UART_Data;

    /**
     * @enum UART_Data_t
     * @brief UART receive buffer status indicators.
     *
     * @details
     * This enumeration represents the state of the UART receive operation:
     * - UART_Empty: No new data available
     * - UART_Received: Valid data byte received
     * - UART_Fault: Receive error detected
     */
    enum UART_Data_t
    {
        UART_Empty = 0,     /**< No data available in receive buffer */
        UART_Received,      /**< Valid data byte successfully received */
        UART_Fault          /**< Receive operation failed (error condition) */
    };

    /**
     * @typedef UART_Error
     * @brief Alias for enum UART_Error_t representing UART error status.
     */
    typedef enum UART_Error_t UART_Error;

    /**
     * @enum UART_Error_t
     * @brief UART frame error codes.
     *
     * @details
     * This enumeration defines specific error conditions detected during UART reception:
     * Corresponds to USART status register error flags (FE, DOR, UPE).
     */
    enum UART_Error_t
    {
        UART_None = 0,      /**< No UART error detected */
        UART_Frame,         /**< Framing error (invalid stop bit) */
        UART_Overrun,       /**< Data overrun (RX buffer overflow) */
        UART_Parity         /**< Parity error (invalid parity bit) */
    };

    /**
     * @typedef UART_Handshake
     * @brief Alias for enum UART_Handshake_t representing flow control status.
     */
    typedef enum UART_Handshake_t UART_Handshake;

    /**
     * @enum UART_Handshake_t
     * @brief UART hardware/software flow control states.
     *
     * @details
     * This enumeration represents the flow control/handshake status between communicating devices:
     * Used for RTS/CTS hardware handshake or XON/XOFF software flow control.
     */
    enum UART_Handshake_t
    {
        UART_Status = 0,    /**< Initial/unknown handshake state */
        UART_Ready,         /**< Ready to send/receive data */
        UART_Pause          /**< Pause transmission (flow control active) */
    };

#endif /* UART_ENUM_H_ */