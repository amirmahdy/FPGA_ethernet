module dvd(clk20, Ethernet_TDp, Ethernet_TDm ,CLKOUT , DataIN , TC , CE );

input clk20;
input [7:0] DataIN ;
output Ethernet_TDp,Ethernet_TDm;
output TC , CE ,CLKOUT ;
wire [7:0] DATAADC;
reg [15:0] ACCADD = 0;
reg [7:0] ACC = 0;
reg [7:0] TMP = 0; 
TENBASET_TxD trn_inst(clk20, Ethernet_TDp, Ethernet_TDm,DATAADC); 

//************** ADC REQUIREMENTS *****************

assign CLKOUT = clk20;
assign TC = 1; assign CE = 0;
assign DATAADC = ACC;
always @(posedge clk20)
begin
ACCADD = ACCADD + DataIN;

if (TMP == 8'h80)
begin
ACC = ACCADD/8'h80 ;
ACCADD = 0;
TMP = 0;
end
TMP = TMP + 1;
end

endmodule


