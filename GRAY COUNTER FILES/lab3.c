 

	// Add user logic here
 led_user_logic # (
    .C_S_AXI_ADDR_WIDTH(C_S_AXI_ADDR_WIDTH), // parameters
    .ADDR_LSB(ADDR_LSB)
    ) U1 (
    .S_AXI_ACLK(S_AXI_ACLK),
    .S_AXI_ARESETN(S_AXI_ARESETN),
    .slv_reg_wren(slv_reg_wren),
    .axi_awaddr(axi_awaddr[C_S_AXI_ADDR_WIDTH-1 : ADDR_LSB]),
    .S_AXI_WDATA(S_AXI_WDATA),
    .LED(LED)
    );
    // User logic ends

    endmodule
    
    
   ///////////////////////////////////////////////////////////
   // Module Name: led_user_logic
   /////////////////////////////////////////////////////////////////
   module led_user_logic#(
        parameter C_S_AXI_ADDR_WIDTH = 4,
        parameter ADDR_LSB = 2)(
       input S_AXI_ACLK,
       input S_AXI_ARESETN,
       input slv_reg_wren,
       input [C_S_AXI_ADDR_WIDTH-1 :ADDR_LSB] axi_awaddr,
       input [31:0] S_AXI_WDATA,
       output  [7:0] LED
       );
       wire rst_FSM_pulse,rst_gray,enable_pulse;
       
      WData_out WData_out(
      .S_AXI_ACLK(S_AXI_ACLK),
      .S_AXI_ARESETN(S_AXI_ARESETN),
      .slv_reg_wren(slv_reg_wren),
      .axi_awaddr(axi_awaddr),
      .S_AXI_WDATA(S_AXI_WDATA),
      .enable_pulse(enable_pulse),
      .rst_FSM_pulse(rst_FSM_pulse),
      .rst_gray(rst_gray)
      );
      
      /*gray_Controller gray_Controller_inst(
                    .clk(S_AXI_ACLK), 
                    .rst(S_AXI_ARESETN),
                    .Data_out(Data_out[1:0]),
                    .enable_pulse(enable_pulse),
                    .rst_FSM_pulse(rst_FSM_pulse),
                    .rst_gray(rst_gray),
                    );*/
                    
      GrayCounter_System  System(
                        .clk(S_AXI_ACLK),
                        .rst(S_AXI_ARESETN),
                        .enable_pulse(enable_pulse),
                        .rst_FSM_pulse(rst_FSM_pulse),
                        .rst_gray(rst_gray),
                        .leds(LED)
                        );

   endmodule
      //////////////////////////////////////////////////////////////////////////////////
   // Company: Uth Un.
   // Engineer: 
   // 
   // Create Date:    18:50:26 25/2/2019
   // Design Name:    Theologis Anthimopoulos ,Dimitris Christodoulou
   // Module Name:     WData_out 
   // Project Name: 
   // Target Devices: 
   // Tool versions: 
   // Description: 
   //
   // Dependencies: 
   //
   // Revision: 
   // Revision 0.01 - File Created
   // Additional Comments: When the AXI DATA are valid take them to the system
   //
   //////////////////////////////////////////////////////////////////////////////////
   module WData_out #(
    parameter C_S_AXI_ADDR_WIDTH = 4,
    parameter ADDR_LSB = 2)
    (
    input S_AXI_ACLK,
    input S_AXI_ARESETN,
    input slv_reg_wren,
    input [C_S_AXI_ADDR_WIDTH-1 :ADDR_LSB] axi_awaddr,
    input [31:0] S_AXI_WDATA,
    output reg enable_pulse,rst_FSM_pulse,rst_gray
    );
    
    always @(posedge S_AXI_ACLK)
        begin 
            if (S_AXI_ARESETN == 1'b0)
                begin 
                    enable_pulse = 'b0;
                    rst_FSM_pulse = 'b0;
                    rst_gray = 'b0;                  
                end 
            else 
                if (slv_reg_wren && (axi_awaddr == 0))
                   begin
                     case(S_AXI_WDATA [1:0])
                       2'd00 :
                              begin 
                                    enable_pulse = 'b0;
                                   rst_FSM_pulse = 'b0;
                                   rst_gray = 'b0;
                              end 
                       2'd01 : 
                             begin 
                                   enable_pulse = 'b0;
                                   rst_FSM_pulse = 'b1;
                                   rst_gray = 'b0;
                             end 
                       2'd10 :
                             begin 
                                   enable_pulse = 'b1;
                                   rst_FSM_pulse = 'b0;
                                   rst_gray = 'b0;
                             end 
                       2'd11 :
                             begin 
                                   enable_pulse = 'b0;
                                   rst_FSM_pulse = 'b1;
                                   rst_gray = 'b1;
                             end 
                       default : 
                               begin 
                                   enable_pulse = 'b0;
                                   rst_FSM_pulse = 'b0;
                                   rst_gray = 'b0;
                              end 
                   endcase
                  end 
                else 
                    begin 
                        enable_pulse = 'b0;
                        rst_FSM_pulse = 'b0;
                        rst_gray = 'b0;                  
                    end 
        end 
endmodule
   //////////////////////////////////////////////////////////////////////////////////
   // Company: Uth Un.
   // Engineer: 
   // 
   // Create Date:    18:50:26 25/2/2019
   // Design Name:    Theologis Anthimopoulos ,Dimitris Christodoulou
   // Module Name:    Led_Cotroller 
   // Project Name: 
   // Target Devices: 
   // Tool versions: 
   // Description: 
   //
   // Dependencies: 
   //
   // Revision: 
   // Revision 0.01 - File Created
   // Additional Comments: When Data_out is valid enable or disable rst and enable signals
   //                            for one clk syrcle(Data_ouut is valid for one clk syrckle)
   //////////////////////////////////////////////////////////////////////////////////

/*module gray_Controller(clk,rst,Data_out,enable_pulse,rst_FSM_pulse,rst_gray);
    input clk , rst;
    input [1:0]Data_out;
    output reg enable_pulse,rst_FSM_pulse,rst_gray;
    //reg reg_enable_pulse,reg_rst_FSM_pulse,reg_rst_gray
    
    //assign enable_pulse = reg_enable_pulse;
    //assign rst_FSM_pulse = reg_rst_FSM_pulse;
    //assign rst_gray = reg_rst_gray;
    always @(posedge clk or posedge rst)
        begin 
            if (rst)
               begin 
                 enable_pulse = 'b0;
                 rst_FSM_pulse = 'b0;
                 rst_gray = 'b0;
               end 
            else 
                case(Data_out)
                    2'd00 :
                           begin 
                                 enable_pulse = 'b0;
                                rst_FSM_pulse = 'b0;
                                rst_gray = 'b0;
                           end 
                    2'd01 : 
                          begin 
                                enable_pulse = 'b0;
                                rst_FSM_pulse = 'b1;
                                rst_gray = 'b0;
                          end 
                    2'd10 :
                          begin 
                                enable_pulse = 'b1;
                                rst_FSM_pulse = 'b0;
                                rst_gray = 'b0;
                          end 
                    2'd11 :
                          begin 
                                enable_pulse = 'b0;
                                rst_FSM_pulse = 'b1;
                                rst_gray = 'b1;
                          end 
                    default : 
                            begin 
                                enable_pulse = 'b0;
                                rst_FSM_pulse = 'b0;
                                rst_gray = 'b0;
                           end 
                endcase
        end 
endmodule*/
   //////////////////////////////////////////////////////////////////////////////////
   // Company: Uth Un.
   // Engineer: 
   // 
   // Create Date:    18:50:26 25/2/2019
   // Design Name:    Theologis Anthimopoulos ,Dimitris Christodoulou
   // Module Name:    GrayCounter_System 
   // Project Name: 
   // Target Devices: 
   // Tool versions: 
   // Description: 
   //
   // Dependencies: 
   //
   // Revision: 
   // Revision 0.01 - File Created
   // Additional Comments: Instantiation of the GrayCounter_Nbits and GrayCounter_Pulse models
   //
   //////////////////////////////////////////////////////////////////////////////////
   module GrayCounter_System(clk, rst,enable_pulse,rst_FSM_pulse,rst_gray, leds);
     parameter N = 8;
     parameter distance = 100000000;// How much for 1 Hz when clk period is 10 ns?
     
     input clk, rst,enable_pulse,rst_FSM_pulse,rst_gray;
     output [N-1:0] leds;
     
     wire pulse;

     // Instantiation of the GrayCounter_Pulse 
     // Here
       GrayCounter_Pulse GrayCounter_Pulse(.clk(clk), .rst(rst), .level(enable_pulse),.rst_FSM_pulse(rst_FSM_pulse), .pulse(pulse));
    
     // Instantiation of the gray_Nbits 
     // Here
       gray_Nbits gray_Nbits(.clk(clk), .clk_en(pulse), .rst(rst),.rst_gray(rst_gray), .gray_out(leds)); 
   
   endmodule
    /*
   -----------------------------------------------------------------------------
   -- File           : Gray_Nbits_RTL.v
   -----------------------------------------------------------------------------
   */ 
  
  module gray_Nbits (clk, clk_en, rst,rst_gray, gray_out);
    parameter N = 8;
    parameter SIZE =(N+1);
    parameter Zeros = {SIZE{1'b0}};
    
    input clk,rst,clk_en,rst_gray;
    output [N-1:0] gray_out;
    
   
    // Place the definition of wires and regs here
    reg [N:0] toggle,state;
    reg temp_or;
    integer i,j,k;
    
    // The state of the Gray counter
    always @(posedge clk or negedge rst)
      begin
           if (rst == 1'b0)
              // Initialize state with 1000..00
              state <= {1'b1,{N{1'b0}}}; 
           else
              begin
                if (rst_gray)
                    state <= {1'b1,{N{1'b0}}};
                else 
                  if (clk_en)
                      begin
                      for (k=0;k<N+1;k=k+1)
                          begin  
                              if (toggle[k]) 
                                  state[k] <= ~state[k]; 
                          end 
                      end
              end
      end
    
    
    // The combinational logic produces the toggle[N:0] signals
    always @(state) 
      begin     
          toggle[0] =  1'b1;
          toggle[1] = state[0];
            toggle [N] = (state[N-2:0] == 'b0) ? 1'b1 :1'b0;
          for (i=2; i<N; i=i+1) 
              begin    
                  if ((state[i-1] == 1'b1))
                      begin 
                          for (j = 0; j<(i-1); j=j+1)
                                begin 
                                  if (j == 0)
                                      temp_or = state[j];
                                  else 
                                      temp_or = temp_or|state[j];
                                end  
                              toggle[i] = (temp_or == 'd0) ? 1'd1 : 1'd0;
                      end
                  else 
                      toggle[i] = 1'b0;
              end 
      end
      
    assign gray_out=state[N:1];
       
  endmodule
  //////////////////////////////////////////////////////////////////////////////////
  // Company: 
  // Engineer: 
  // 
  // Create Date:    18:17:55 25/2/2010 
  // Design Name:    Theologis Anthimopoulos ,Dimitris Christodoulou
  // Module Name:    GrayCounter_Nbit_Pulse 
  // Project Name: 
  // Target Devices: 
  // Tool versions: 
  // Description: This FSM converts an input level signal to a single-cycle pulse. 
  //
  // Dependencies: 
  //
  // Revision: 
  // Revision 0.01 - File Created
  // Additional Comments: When level signal (level) pulse get enabled and FSM will go to stayte s1 .After a cycle FSM
  //        will go to state2 (S2) and pulse will be zero . After FSN will start from state zero (S0)
  //////////////////////////////////////////////////////////////////////////////////
  module GrayCounter_Pulse(clk, rst, level,rst_FSM_pulse, pulse);
      input clk, rst, level,rst_FSM_pulse;
      output reg pulse;
      
     reg [1:0] state;
     reg [1:0] nextstate;
     parameter S0 = 2'b00;
     parameter S1 = 2'b01;
     parameter S2 = 2'b10;
     
     // Compute next state of the FSM 
    always @(state or level)
        begin
              pulse = 'd0;
              nextstate = state;
              case(state)
                  S0:
                  begin
                      if (level)
                      begin
                          pulse = 'd1;
                          nextstate = S1;
                      end
                      else
                          nextstate = S0;
                  end
                  S1:
                  begin
                      nextstate = S2;
                  end
                  S2:
                  begin
                      if (level == 'd0)
                          nextstate = S0;
                  end
                  default: nextstate = S0;
              endcase
        end
        
        // Set the new state 
      always @(posedge clk or negedge  rst)
              begin
                  if(rst == 0)
                      state <= S0;
                  else
                     if (rst_FSM_pulse)
                         state <= S0;
                     else 
                         state <= nextstate;
      end
  endmodule
#include "xparameters.h"
#include "xgpio.h"
#include "led_ip.h"
//====================================================

int main (void) 
{

   XGpio dip, push;
   int i, psb_check, dip_check;
   int counter, button_value;
	
   xil_printf("-- Start of the Program --\r\n");
  
   XGpio_Initialize(&dip, XPAR_SW_8BIT_DEVICE_ID); // Modify this
   XGpio_SetDataDirection(&dip, 1, 0xffffffff);
	
   XGpio_Initialize(&push, XPAR_BTNS_5BIT_DEVICE_ID); // Modify this
   XGpio_SetDataDirection(&push, 1, 0xffffffff);
	

   while (1)
   {
	  psb_check = XGpio_DiscreteRead(&push, 1);
	  //xil_printf("Push Buttons Status %x\r\n", psb_check);
	  dip_check = XGpio_DiscreteRead(&dip, 1);
	  //xil_printf("DIP Switch Status %x\r\n", dip_check);
	  
      if (psb_check == 1){
    	  LED_IP_mWriteReg(XPAR_LED_IP_0_S_AXI_BASEADDR, 0, 1);
    	  button_value = 1;
      }
      else if (psb_check == 2){
    	  LED_IP_mWriteReg(XPAR_LED_IP_0_S_AXI_BASEADDR, 0, 2);
    	  button_value = 2;
      }
      else if (psb_check == 3){
    	  LED_IP_mWriteReg(XPAR_LED_IP_0_S_AXI_BASEADDR, 0, 3);
    	  button_value = 3;
      }
      else {
    	  LED_IP_mWriteReg(XPAR_LED_IP_0_S_AXI_BASEADDR, 0, button_value);
      }
	  // output dip switches value on LED_ip device

      counter =  LED_IP_mReadReg(XPAR_LED_IP_0_S_AXI_BASEADDR, LED_IP_S_AXI_SLV_REG1_OFFSET);

      xil_printf("Counter %d\r\n", counter);
    
	  for (i=0; i<9999999; i++);
   }
}
