#include <millisDelay.h>
#include "RTC_SAMD51.h"
#include "DateTime.h"
#include "common.h"

unsigned long getNTPtime();
unsigned long sendNTPpacket(const char* address);

struct timeDef systemTime; 

millisDelay updateDelay; // the update delay object. used for ntp periodic update.
unsigned int localPort = 2390;      // local port to listen for UDP packets
// switch between local and remote time servers
// comment out to use remote server
//#define USELOCALNTP
#ifdef USELOCALNTP
    char timeServer[] = "n.n.n.n"; // local NTP server 
#else
    char timeServer[] = "time.nist.gov"; // extenral NTP server e.g. time.nist.gov
#endif
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

DateTime now;//declare a time object
WiFiUDP udp;//The udp library class
RTC_SAMD51 rtc;
extern WiFiClient client;
unsigned long devicetime;//localtime

//for use by the Adafuit RTClib library
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
 
void setTime(void) {
    // get the time via NTP (udp) call to time server
    // getNTPtime returns epoch UTC time adjusted for timezone but not daylight savings
    // time
    devicetime = getNTPtime();
 
    // check if rtc present
    if (devicetime == 0) {
        Serial.println("Failed to get time from network time server.");
    }
 
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1) delay(10); // stop operating
    }
 
    // adjust time using ntp time
    rtc.adjust(DateTime(devicetime));
    // get and print the adjusted rtc time
    now = rtc.now();
    Serial.print("Adjusted RTC (boot) time is: ");
    Serial.println(now.timestamp(DateTime::TIMESTAMP_FULL));
 
    // start millisdelays timers as required, adjust to suit requirements
    updateDelay.start(12 * 60 * 60 * 1000); // update time via ntp every 12 hrs
}
 
void updateNTPtime() {
    //获取系统使用的时间
    String str = now.timestamp(DateTime::TIMESTAMP_FULL);
    strncpy(systemTime.nowDay, str.c_str(), 10); //日期
    strncpy(systemTime.nowTime, (str.c_str())+12, 8); //时间

    if (updateDelay.justFinished()) { // 12 hour loop
        // repeat timer
        updateDelay.repeat(); // repeat
 
        // update rtc time
        devicetime = getNTPtime();
        if (devicetime == 0) {
            Serial.println("Failed to get time from network time server.");
        }
        else {
            rtc.adjust(DateTime(devicetime));
            Serial.println("");
            Serial.println("rtc time updated.");
            // get and print the adjusted rtc time
            now = rtc.now();
            Serial.print("Adjusted RTC time is: ");
            Serial.println(now.timestamp(DateTime::TIMESTAMP_FULL));
        }
    }
}
 
 
unsigned long getNTPtime() {
 
    // module returns a unsigned long time valus as secs since Jan 1, 1970 
    // unix time or 0 if a problem encounted
 
    //only send data when connected
    if (WiFi.status() == WL_CONNECTED) {
        //initializes the UDP state
        //This initializes the transfer buffer
        udp.begin(WiFi.localIP(), localPort);
 
        sendNTPpacket(timeServer); // send an NTP packet to a time server
        // wait to see if a reply is available
        delay(1000);
        if (udp.parsePacket()) {
            Serial.println("udp packet received");
            Serial.println("");
            // We've received a packet, read the data from it
            udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
 
            //the timestamp starts at byte 40 of the received packet and is four bytes,
            // or two words, long. First, extract the two words:
 
            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            unsigned long secsSince1900 = highWord << 16 | lowWord;
            // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
            const unsigned long seventyYears = 2208988800UL;
            // subtract seventy years:
            unsigned long epoch = secsSince1900 - seventyYears;
 
            // adjust time for timezone offset in secs +/- from UTC
            // WA time offset from UTC is +8 hours (28,800 secs)
            // + East of GMT
            // - West of GMT
            long tzOffset = 28800UL;
 
            // WA local time 
            unsigned long adjustedTime;
            return adjustedTime = epoch + tzOffset;
        }
        else {
            // were not able to parse the udp packet successfully
            // clear down the udp connection
            udp.stop();
            return 0; // zero indicates a failure
        }
        // not calling ntp time frequently, stop releases resources
        udp.stop();
    }
    else {
        // network not connected
        return 0;
    }
 
}
 
// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(const char* address) {
    // set all bytes in the buffer to 0
    for (int i = 0; i < NTP_PACKET_SIZE; ++i) {
        packetBuffer[i] = 0;
    }
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
 
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    udp.beginPacket(address, 123); //NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
}