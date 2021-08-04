                                                  /*Anyone is free to copy, modify, publish, use, compile or
                                                    distribute this software, either in source code form or as a compiled
                                                    binary, for non-commercial use only. (i.e. YOU MAY NOT SELL IT)
                                                    John B 04/08/2021

                                                    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
                                                    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
                                                    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
                                                    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
                                                    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
                                                    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
                                                    OTHER DEALINGS IN THE SOFTWARE.*/


                                             // Screen Capture and writing to SRAM.
                                             // Read SRAM and display on screen.
                                             // Capture function to put in Main.


unsigned int dummybuffer = 0;
void Screen_Capture(unsigned int Start_Column, unsigned int End_Column, unsigned int Start_Row, unsigned int End_Row){

     unsigned long j = 0;
     TFT_CS = 0;
     SRAM_CS = 0;

     Write_Command_SSD1963(0x2A);           //: Set Start Column               D/C = 0;
     Write_Data_SSD1963(Start_column>>8);   //: Start Column Number High Byte  D/C = 1;
     Write_Data_SSD1963(Start_column);      //: Start Column Number Low Byte   D/C = 1;
     Write_Data_SSD1963(End_Column>>8);     //: End Column Number High Byte    D/C = 1;
     Write_Data_SSD1963(End_Column);        //: End Column Number Low Byte     D/C = 1;

     Write_Command_SSD1963(0x2B);           //: SET ROW ADDRESS                D/C = 0;
     Write_Data_SSD1963(Start_Row>>8);      //: Start Row Number High Byte     D/C = 1;
     Write_Data_SSD1963(Start_Row);         //: Start Row Number Low Byte      D/C = 1;
     Write_Data_SSD1963(End_Row>>8);        //: End Row Number High Byte       D/C = 1;
     Write_Data_SSD1963(End_Row);           //: End Row Number Low Byte        D/C = 1;

     Write_Command_SSD1963(0x2E);// Read Memory Start

      for(j=0; j<384000; j++){ //800x480
       dummybuffer = PMRDIN;
       DMA_EBI_Start_Increment(j);
       while(PMMODE & 0x8000); //Busy bit
      }
           TFT_CS = 1;
           SRAM_CS = 1;
           DMA_EBI_Start_Increment(0);
}




void Write_Screen_From_SRAM(unsigned int Start_Column, unsigned int End_Column, unsigned int Start_Row, unsigned int End_Row){

         unsigned long j=0;

         SRAM_CS = 0;
         TFT_CS = 0;

         Write_Command_SSD1963(0x2A);           //: Set Start Column               D/C = 0;
         Write_Data_SSD1963(Start_column>>8);   //: Start Column Number High Byte  D/C = 1;
         Write_Data_SSD1963(Start_column);      //: Start Column Number Low Byte   D/C = 1;
         Write_Data_SSD1963(End_Column>>8);     //: End Column Number High Byte    D/C = 1;
         Write_Data_SSD1963(End_Column);        //: End Column Number Low Byte     D/C = 1;

         Write_Command_SSD1963(0x2B);           //: SET ROW ADDRESS                D/C = 0;
         Write_Data_SSD1963(Start_Row>>8);      //: Start Row Number High Byte     D/C = 1;
         Write_Data_SSD1963(Start_Row);         //: Start Row Number Low Byte      D/C = 1;
         Write_Data_SSD1963(End_Row>>8);        //: End Row Number High Byte       D/C = 1;
         Write_Data_SSD1963(End_Row);           //: End Row Number Low Byte        D/C = 1;

         Write_Command_SSD1963(0x2C);   // Write Memory Start, 0x2C
         Write_Data_SSD1963(0x2C);   // Write Memory Start, 0x2C  9.30 Write Memory Continue    Command 0x3C

         PMCONbits.PTWREN = 0;   //TURN OFF AUTO WRITE STROBE ENABLE

          for (j=0; j<384000; j++){

            PMDOUT = dummybuffer;
            RC3_Bit = 0;    //Write Bit Off
            RC3_Bit = 1;    //Write Bit On
            DMA_EBI_Start_Increment(j);
           }

         TFT_CS = 1;
         SRAM_CS = 1;
         DMA_EBI_Start_Increment(0);
         PMCONbits.PTWREN = 1;   //TURN ON AUTO WRITE STROBE ENABLE
}


 Screen_Capture(Capture_Start_Column,Capture_End_Column,Capture_Start_Row,Capture_End_Row);