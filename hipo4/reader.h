//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */
 /**
  * <pre>
  *
  * FILE HEADER STRUCTURE ( 56 bytes, 14 integers (32 bit) )
  *
  *    +----------------------------------+
  *  1 |              ID                  | // HIPO: 0x43455248, Evio: 0x4556494F
  *    +----------------------------------+
  *  2 +          File Number             | // split file #
  *    +----------------------------------+
  *  3 +         Header Length            | // 14 (words)
  *    +----------------------------------+
  *  4 +      Record (Index) Count        |
  *    +----------------------------------+
  *  5 +      Index Array Length          | // bytes
  *    +-----------------------+----------+
  *  6 +       Bit Info        | Version  | // version (8 bits)
  *    +-----------------------+----------+
  *  7 +      User Header Length          | // bytes
  *    +----------------------------------+
  *  8 +          Magic Number            | // 0xc0da0100
  *    +----------------------------------+
  *  9 +          User Register           |
  *    +--                              --+
  * 10 +                                  |
  *    +----------------------------------+
  * 11 +         Trailer Position         | // File offset to trailer head (64 bits).
  *    +--                              --+ // 0 = no offset available or no trailer exists.
  * 12 +                                  |
  *    +----------------------------------+
  * 13 +          User Integer 1          |
  *    +----------------------------------+
  * 14 +          User Integer 2          |
  *    +----------------------------------+
  *
  * -------------------
  *   Bit Info Word
  * -------------------
  *     0-7  = version
  *     8    = true if dictionary is included (relevant for first record only)
  *     9    = true if this file has "first" event (in every split file)
  *    10    = File trailer with index array exists
  *    11-19 = reserved
  *    20-21 = pad 1
  *    22-23 = pad 2
  *    24-25 = pad 3 (always 0)
  *    26-27 = reserved
  *    28-31 = general header type: 1 = Evio file
  *                                 2 = Evio extended file
  *                                 5 = HIPO file
  *                                 6 = HIPO extended file
  *
  * </pre>
  *
  * @version 6.0
  * @since 6.0 9/6/17
  */
/*
 * File:   reader.h
 * Author: gavalian
 *
 * Created on April 11, 2017, 2:07 PM
 */

#ifndef HIPOREADER_H
#define HIPOREADER_H


#define HIPO_FILE_HEADER_SIZE 72
/* Constants for endianness of the file */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN     0
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN  1
#endif

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <climits>
#include "record.h"
#include "utils.h"
#include "bank.h"

namespace hipo {

  typedef struct {
    int  uniqueid{};
    int  filenumber{};
    int  headerLength{}; // in words (usually 14)
    int  recordCount{};
    int  indexArrayLength{}; // in bytes
    int  bitInfo{};
    int  version{};
    int  userHeaderLength{};
    int  magicNumber{};
    long userRegister{};
    long trailerPosition{};
    long firstRecordPosition{};
  } fileHeader_t;


  typedef struct {
      long recordPosition{};
      int  recordLength{};
      int  recordEntries{};
      long userWordOne{};
      long userWordTwo{};
  } recordInfo_t;


  /**
  * READER index class is used to construct entire events
  * sequence from all records, and provides ability to canAdvance
  * through events where record number is automatically calculated
  * and triggers reading of the next record when events in the current
  * record are exhausted.
  */

  class readerIndex {

    private:
      std::vector<int>  recordEvents;
      std::vector<long> recordPosition;

      int              currentRecord{};
      int              currentEvent{};
      int              currentRecordEvent{};

   public:

      readerIndex()= default;;
      ~readerIndex()= default;;

      bool canAdvance();
      bool advance();

      //dglazier
      bool canAdvanceInRecord();
      bool loadRecord(int irec);
      bool  gotoEvent(int eventNumber);
      bool  gotoRecord(int irec);

      int  getEventNumber() { return currentEvent;}
      int  getRecordNumber() { return currentRecord;}
      int  getRecordEventNumber() { return currentRecordEvent;}
      int  getMaxEvents();
      void addSize(int size);
      void addPosition(long position){ recordPosition.push_back(position);}
      long getPosition(int index) { return recordPosition[index];}

      //dglazier
      int getNRecords() const {return recordEvents.size();}

      void rewind(){
        currentRecord = -1; currentEvent  = -1; currentRecordEvent = -1;
      }
      void clear(){
        recordEvents.clear(); recordPosition.clear();
      }
      void reset(){
        currentRecord = 0; currentEvent  = 0; currentRecordEvent = 0;
      }
};

  class reader {

    private:

        fileHeader_t      header{};
        hipo::utils       hipoutils;
        std::ifstream     inputStream;
        long              inputStreamSize{};

        hipo::record       inputRecord;
        hipo::readerIndex  readerEventIndex;
        std::vector<long>  tagsToRead;
	short _verbose = {0} ;
	
        void  readHeader();
        void  readIndex();
    public:

        reader();
        reader(const reader &r){}

        ~reader();

        void  about();
        void  readDictionary(hipo::dictionary &dict);
        void  getStructure(hipo::structure &structure,int group, int item);
        void  getStructureNoCopy(hipo::structure &structure,int group, int item);

        void  open(const char *filename);
        void  setTags(int tag){ tagsToRead.push_back(tag);}
	void  setTags(std::vector<long> tags){ tagsToRead=std::move(tags);}
	void  setVerbose(short level=1){_verbose=level;}
	      
	bool  hasNext();
        bool  next();
        bool  gotoEvent(int eventNumber);
        bool  gotoRecord(int irec);
        bool  next(hipo::event &dataevent);
        void  read(hipo::event &dataevent);
        void  printWarning();

	int getNRecords() const {return readerEventIndex.getNRecords()-1;}
	bool  nextInRecord();
	bool loadRecord(int irec);
	int  getEntries(){return readerEventIndex.getMaxEvents();}
  };
}
#endif /* HIPOREADER_H */
