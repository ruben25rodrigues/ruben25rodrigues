static void Main(){

int contador=0;

for (int f=100; f>=-10; f--;){

double c= 5.0/9.0*(f-32.0);

Console.WriteLine($"{f} degrees Fahrenheit correspond to {c} degrees Celsius");

contador++;
if (contador %10==0) Console.WriteLine("");
}
     
}