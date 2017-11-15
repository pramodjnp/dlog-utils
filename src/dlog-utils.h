#ifndef DLOG_CONVERT_H
#define DLOG_CONVERT_H

#include <string>
#include <vector>
#include <cstdint>
#include <math.h>

#ifdef DEBUGPRINT 
    #define DEBUGLN(x) (std::cout << (x) << std::endl)
    #define DEBUG(x) (std::cout << (x))
#else 
    #define DEBUG(x)
    #define DEBUGLN(x)
#endif

namespace DlogUtil
{
/******************************************************************************
* DlogConvert status enum
******************************************************************************/
enum ConvertStatus: uint32_t
{
    OK = 0,
    FILEOPENERR,
    INVALIDHEADER
};

/******************************************************************************
* DlogConvert class
* 
* This class exposes the ability to convert dlog files into csv files
******************************************************************************/
class DlogConvert
{
private:
        /******************************************************************************
        * File header info struct
        ******************************************************************************/
        struct HeaderInfo
        {
            uint8_t  endian;          // 0 - little endian 1 - big endian
            uint8_t  cbSampleEntry;   // how many bytes per sample, OpenScope = 2
            uint16_t cbHeader;        // how long is this header structure
            uint16_t cbHeaderInFile;  // how much space is taken in the file for the header, sector aligned (512)
            uint16_t format;          // General format of the header and potential data
            uint32_t revision;        // specific header revision (within the general format)
            uint64_t voltageUnits;    // divide the voltage of each sample by this to get volts.
            uint32_t stopReason;      // reason for logging stopping; 0 = Normal, 1 = Forced, 2 = Error, 3 = Overflow, 4 = unknown
            uint64_t iStart;          // what sample index is the first index in the file, usually 0
            uint64_t actualCount;     // how many samples in the file.
            uint64_t sampleFreqUnits; // divide uSPS by sampleFreqUnits to get samples per second
            uint64_t uSPS;            // sample rate in micro samples / second
            uint64_t delayUnits;      // divide psDelay by delayUnits to get the delay in seconds.
            int64_t  psDelay;         // how many pico seconds a delay from the start of sampling until the first sample was taken, usually 0
        };
        /******************************************************************************
        * Header info instance
        ******************************************************************************/
        HeaderInfo headerInfo;

        /******************************************************************************
        * Buffer size constants
        ******************************************************************************/
        static const unsigned int BUFFERSIZE = 10000000;
        static const unsigned int OUTBUFFERSIZE = 200000000;

        /******************************************************************************
        * Input and output buffers
        ******************************************************************************/
        uint8_t* fileBuffer = new uint8_t[BUFFERSIZE];
        char* outputBuffer = new char[OUTBUFFERSIZE];

        /******************************************************************************
        * Parse file header
        * @return - Returns a ConvertStatus
        ******************************************************************************/
        ConvertStatus parseFileHeader();

        /******************************************************************************
        * Convert dlog to csv
        * @return - Returns a ConvertStatus
        ******************************************************************************/
        ConvertStatus convertToCsv(std::istream& inputFS, std::ostream& outputFS);

        /******************************************************************************
        * Parse double and place characters in the output buffer
        * @return - void
        ******************************************************************************/
        void doubleToString(double num, char* str, uint8_t precision, uint64_t* numBytesFilled);
    
    public:
        /******************************************************************************
        * Constructor
        ******************************************************************************/
        DlogConvert();

        /******************************************************************************
        * Destructor
        ******************************************************************************/
        ~DlogConvert();

        /******************************************************************************
        * Convert a dlog file to a csv
        * @param - path to dlog file
        * @param - output file name
        * @return - status code
        ******************************************************************************/
        uint32_t ConvertFile(const std::string& pathToDlog, const std::string& outputName);
};
} //namespace DlogUtil
#endif //DLOG_CONVERT_H