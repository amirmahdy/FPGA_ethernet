module TENBASET_TxD(clk20, Ethernet_TDp, Ethernet_TDm,DataIN);
input clk20;
input [7:0] DataIN;
output Ethernet_TDp, Ethernet_TDm;
// "IP source
parameter IPsource_1 = 192;
parameter IPsource_2 = 168;
parameter IPsource_3 = 0;
parameter IPsource_4 = 44;
// "IP destination"
parameter IPdestination_1 = 192;
parameter IPdestination_2 = 168;
parameter IPdestination_3 = 0;
parameter IPdestination_4 = 2;
// "Physical Address"
parameter PhysicalAddress_1 = 8'h00;
parameter PhysicalAddress_2 = 8'h24;
parameter PhysicalAddress_3 = 8'hE8;
parameter PhysicalAddress_4 = 8'hE0;
parameter PhysicalAddress_5 = 8'hC7;
parameter PhysicalAddress_6 = 8'hC2;

//////////////////////////////////////////////////////////////////////
reg [17:0] counter; always @(posedge clk20) counter<=counter+1;
reg StartSending; always @(posedge clk20) StartSending<=&counter;

//////////////////////////////////////////////////////////////////////
// calculate the IP checksum, big-endian style
parameter number = 16'h05C0 ;
parameter PayloadSize = number + 8 ;
parameter CnctSize = number + 8'h1C ;

parameter IPchecksum1 = 32'h0000C52D + number + (IPsource_1<<8)+IPsource_2+(IPsource_3<<8)+IPsource_4+
                                                                (IPdestination_1<<8)+IPdestination_2+(IPdestination_3<<8)+(IPdestination_4);
parameter IPchecksum2 =  ((IPchecksum1&32'h0000FFFF)+(IPchecksum1>>16));
parameter IPchecksum3 = (~((IPchecksum2&32'h0000FFFF)+(IPchecksum2>>16))) ;

reg [15:0] rdaddress;
reg [15:0] pkt_data;

always @(posedge clk20)
case(rdaddress)
// Ethernet preamble
  16'h0000: pkt_data <= 8'h55;
  16'h0001: pkt_data <= 8'h55;
  16'h0002: pkt_data <= 8'h55;
  16'h0003: pkt_data <= 8'h55;
  16'h0004: pkt_data <= 8'h55;
  16'h0005: pkt_data <= 8'h55;
  16'h0006: pkt_data <= 8'h55;
  16'h0007: pkt_data <= 8'hD5;
// Ethernet header
  16'h0008: pkt_data <= PhysicalAddress_1;
  16'h0009: pkt_data <= PhysicalAddress_2;
  16'h000A: pkt_data <= PhysicalAddress_3;
  16'h000B: pkt_data <= PhysicalAddress_4;
  16'h000C: pkt_data <= PhysicalAddress_5;
  16'h000D: pkt_data <= PhysicalAddress_6;
  16'h000E: pkt_data <= 8'h00;
  16'h000F: pkt_data <= 8'h12;
  16'h0010: pkt_data <= 8'h34;
  16'h0011: pkt_data <= 8'h56;
  16'h0012: pkt_data <= 8'h78;
  16'h0013: pkt_data <= 8'h90;
// IP header
  16'h0014: pkt_data <= 8'h08;
  16'h0015: pkt_data <= 8'h00;
  16'h0016: pkt_data <= 8'h45;
  16'h0017: pkt_data <= 8'h00;
  16'h0018: pkt_data <= CnctSize[15:8];
  16'h0019: pkt_data <= CnctSize[7:0];   // ******************  1C + (nom.)
  16'h001A: pkt_data <= 8'h00;
  16'h001B: pkt_data <= 8'h00;
  16'h001C: pkt_data <= 8'h00;
  16'h001D: pkt_data <= 8'h00;
  16'h001E: pkt_data <= 8'h80;
  16'h001F: pkt_data <= 8'h11;
  16'h0020: pkt_data <= IPchecksum3[15:8];
  16'h0021: pkt_data <= IPchecksum3[ 7:0];  // ****************** 
  16'h0022: pkt_data <= IPsource_1;
  16'h0023: pkt_data <= IPsource_2;
  16'h0024: pkt_data <= IPsource_3;
  16'h0025: pkt_data <= IPsource_4;
  16'h0026: pkt_data <= IPdestination_1;
  16'h0027: pkt_data <= IPdestination_2;
  16'h0028: pkt_data <= IPdestination_3;
  16'h0029: pkt_data <= IPdestination_4;
// UDP header
  16'h002A: pkt_data <= 8'h04;
  16'h002B: pkt_data <= 8'h00;
  16'h002C: pkt_data <= 8'h04;
  16'h002D: pkt_data <= 8'h00;
  16'h002E: pkt_data <= PayloadSize[15:8];
  16'h002F: pkt_data <= PayloadSize[7:0]; //  ******* UDP PAYLOAD SIZE *********   (nom.)+8
  16'h0030: pkt_data <= 8'h00;
  16'h0031: pkt_data <= 8'h00;
  default: pkt_data <= DataIN;
endcase

//////////////////////////////////////////////////////////////////////
// and finally the 10BASE-T's magic
reg [3:0] ShiftCount;
reg SendingPacket;
always @(posedge clk20) if(StartSending) SendingPacket<=1; else if(ShiftCount==14 && rdaddress==(16'h0036 + number)) SendingPacket<=0;
always @(posedge clk20) ShiftCount <= SendingPacket ? ShiftCount+1 : 15;
wire readram = (ShiftCount==15);
always @(posedge clk20) if(ShiftCount==15) rdaddress <= SendingPacket ? rdaddress+1 : 0;
reg [7:0] ShiftData; always @(posedge clk20) if(ShiftCount[0]) ShiftData <= readram ? pkt_data : {1'b0, ShiftData[7:1]};

// generate the CRC32
reg [31:0] CRC ;
reg CRCflush; always @(posedge clk20) if(CRCflush) CRCflush <= SendingPacket; else if(readram) CRCflush <= (rdaddress== (16'h0032 + number));
reg CRCinit; always @(posedge clk20) if(readram) CRCinit <= (rdaddress==7);
wire CRCinput = CRCflush ? 0 : (ShiftData[0] ^ CRC[31]);
always @(posedge clk20) if(ShiftCount[0]) CRC <= CRCinit ? ~0 : ({CRC[30:0],1'b0} ^ ({32{CRCinput}} & 32'h04C11DB7));

// generate the NLP
reg [16:0] LinkPulseCount; always @(posedge clk20) LinkPulseCount <= SendingPacket ? 0 : LinkPulseCount+1;
reg LinkPulse; always @(posedge clk20) LinkPulse <= &LinkPulseCount[16:1];

// TP_IDL, shift-register and manchester encoder
reg SendingPacketData; always @(posedge clk20) SendingPacketData <= SendingPacket;
reg [2:0] idlecount; always @(posedge clk20) if(SendingPacketData) idlecount<=0; else if(~&idlecount) idlecount<=idlecount+1;
wire dataout = CRCflush ? ~CRC[31] : ShiftData[0];
reg qo; always @(posedge clk20) qo <= SendingPacketData ? ~dataout^ShiftCount[0] : 1;
reg qoe; always @(posedge clk20) qoe <= SendingPacketData | LinkPulse | (idlecount<6);
reg Ethernet_TDp; always @(posedge clk20) Ethernet_TDp <= (qoe ? qo : 1'b0);
reg Ethernet_TDm; always @(posedge clk20) Ethernet_TDm <= (qoe ? ~qo : 1'b0);

endmodule

