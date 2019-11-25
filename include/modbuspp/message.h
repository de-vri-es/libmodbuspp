/* Copyright © 2018-2019 Pascal JEAN, All rights reserved.
 * This file is part of the libmodbuspp Library.
 *
 * The libmodbuspp Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * The libmodbuspp Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the libmodbuspp Library; if not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <modbuspp/global.h>

namespace Modbus {
  class Device;
  class NetLayer;

  /**
   * @class Message
   * @brief Modbus Message
   *
   * @example server/tcp-gateway/main.cpp
   * 
   * @author Pascal JEAN, aka epsilonrt
   * @copyright GNU Lesser General Public License
   */
  class Message  {
    public:

      friend class Device;
      
      /**
       * @brief Message callback function
       * @param msg
       * @return 1 if the message has been completely processed, 0 if the 
       * message has not been processed, -1 if error.
       */
      typedef int (*Callback) (Message & msg, Device * sender);

      /**
       * @brief Constructors
       */
      Message (NetLayer & backend);
      Message (Device & dev);
      Message (NetLayer & backend, const std::vector<uint8_t> & adu);
      Message (Device & dev, const std::vector<uint8_t> & adu);
      Message (NetLayer & backend, Function func);
      Message (Device & dev, Function func);

      /**
       * @brief Destructor
       */
      virtual ~Message();

      /**
       * @brief Returns the underlying ADU as a byte vector.
       */
      std::vector<uint8_t> & adu ();

      /**
       * @overload 
       */
      const std::vector<uint8_t> & adu () const;
      
      /**
       * @brief Returns the ADU byte corresponding to the index provided
       * 
       * \c i must be between 0 and \c maxAduLength()-1
       */
      uint8_t adu (uint16_t i);

      /**
       * @brief Returns the PDU byte corresponding to the offset provided
       * 
       * \c pduOffset has a maximum value of \c MaxPduLength-1, it can have a negative 
       * value in order to access the bytes of the ADU that precedes the PDU.
       */
      uint8_t byte (int pduOffset) const;

      /**
       * @brief Returns the PDU word corresponding to the offset provided
       * 
       * \c pduOffset has a maximum value of \c MaxPduLength-2, it can have a negative 
       * value in order to access the words of the ADU that precedes the PDU.
       */
      uint16_t word (int pduOffset) const;
      
      /**
       * @brief Write a byte in the PDU to the supplied offset
       * 
       * \c pduOffset has a maximum value of \c MaxPduLength-1, it can have a negative 
       * value in order to access the bytes of the ADU that precedes the PDU.
       */
      void setByte (int pduOffset, uint8_t value);
      
      /**
       * @brief Write a word in the PDU to the supplied offset
       * 
       * \c pduOffset has a maximum value of \c MaxPduLength-2, it can have a negative 
       * value in order to access the wordss of the ADU that precedes the PDU.
       */
       void setWord (int pduOffset, uint16_t value);
      
      /**
       * @brief Returns the number of the slave concerned by the request.
       * 
       * This value is at the offset[-1] of the PDU
       */
      int slave() const;

      /**
       * @brief Returns the function of the request.
       * 
       * This value is at the offset[0] of the PDU
       */
      Function function() const;
      
      
      /**
       * @brief Returns the starting address of the request.
       * 
       * This value is at the offset[1] of the PDU
       */
      uint16_t startingAddress() const;

      /**
       * @brief Returns the number of elements of the request.
       * 
       * This value is at the offset[3] of the PDU
       */
      uint16_t quantity() const;
      
      /**
       * @brief Returns a word value of the request
       * 
       * This value is at the offset[5+index] of the PDU
       */
      uint16_t value (uint16_t index) const;

      /**
       * @brief Sets the number of the slave concerned by the request.
       * 
       * This value is at the offset[-1] of the PDU
       */
      void setSlaveId (int id);

      /**
       * @brief Sets the function of the request.
       * 
       * This value is at the offset[0] of the PDU
       */
      void setFunction (Function func);

      /**
       * @brief Sets the starting address of the request.
       * 
       * This value is at the offset[1] of the PDU
       */
      void setStartingAdress (uint16_t addr);

      /**
       * @brief Sets the number of elements of the request.
       * 
       * This value is at the offset[3] of the PDU
       */
      void setQuantity (uint16_t n);

      /**
       * @brief Underlying layer used (backend)
       *
       * This function allows to know the underlying layer used.
       */
      Net net() const;

      /**
       * @brief Return the maximum ADU length
       */
      uint16_t maxAduLength() const;

    protected:
      class Private;
      Message (Private &dd);
      std::unique_ptr<Private> d_ptr;

    private:
      PIMP_DECLARE_PRIVATE (Message)
  };
}

/* ========================================================================== */
