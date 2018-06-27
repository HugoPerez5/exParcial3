#include <18F4620.h> 
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT 
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG 
#use delay(clock=16000000) 

int display[10] = {63,6,91,79,102,109,125,7,127,103}; 
unsigned int datos[9]={0};
int contadorPortA=1;
int contaUS=0, contaDS=0,contaUM=0,contaDM=0;
int contadorTimer=0;

#int_TIMER0
void timer0_isr(){
   output_a(contadorPortA); //manda por el port a el transistor
   output_d(display[datos[contadorPortA]]);//imprime los datos
  contadorPortA<<=1;//1 2 4 8
   if(contadorPortA>=16){ //regresa a 1
      contadorPortA=1;
   }
}
#int_RB
void rb_isr(){
   if(input(PIN_B4)==1){//unidades segundo
      contaUS++;     
   }
   if(input(PIN_B5)==1){//decenas segundos
       contaDS++;
   }
   if(input(PIN_B6)==1){//unidades minutos
      contaUM++;     
   }
   if(input(PIN_B7)==1){//decenas minutos
       contaDM++;
   }
}

void main (void){ 
setup_oscillator(OSC_16MHZ); 
enable_interrupts(INT_RB);
enable_interrupts(INT_TIMER0);
enable_interrupts(GLOBAL);
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_128|RTCC_8_BIT);
set_tris_a(0x00);
set_tris_b(0x00);
set_tris_c(0x00);
set_tris_d(0x00);


while(1){
    if(contadorTimer==122){ //cada segundo...
        output_c(display[contaUS]);
        contaUS++;//aumenta los unidades segundos
        
        if(contaUS>9){ //si llega a 10, regresa y aumenta las decenas
            contaUS=0;
            datos[8]=contaUS;//imprime en la 4 posicion
            contaDS++;
        }
        if(contaDS>5){//si las decenas de segundos llegan a 6,las UM aumentan
            contaDS=0;
            datos[4]=contaDS;//imprime en la 3 posicion
            contaUM++;            
        }
        if(contaUM>3){//si las unidades de min llegan a 3,las DM aumentan
            contaUM=0;
            datos[2]=contaUS;//imprime en la 2 posicion
            contaDM++;            
        }
        if(contaDM>2){//si las unidades de min llegan a 2,las DM aumentan
            contaDM=0;
            datos[1]=contaUS;//imprime en la 1 posicion            
        }
        contadorTimer=0;
    }
    
}
}