v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 49600 44000 1 90 0 resistor-1.sym
{
T 49200 44300 5 10 0 0 90 0 1
device=RESISTOR
T 49400 44100 5 10 1 1 90 0 1
refdes=R101
T 49600 44000 5 10 0 0 90 0 1
footprint=R025
T 49800 44100 5 10 1 1 90 0 1
value=180
}
C 49900 45600 1 0 0 npn-ECB.sym
{
T 50500 46100 5 10 0 0 0 0 1
device=NPN_TRANSISTOR
T 50500 46100 5 10 1 1 0 0 1
refdes=Q101
T 50500 46300 5 10 0 0 0 0 1
footprint=TO92
T 50500 45800 5 10 1 1 0 0 1
value=C1815
}
C 46100 44100 1 90 0 capacitor-1.sym
{
T 45400 44300 5 10 0 0 90 0 1
device=CAPACITOR
T 45600 44300 5 10 1 1 90 0 1
refdes=C101
T 45200 44300 5 10 0 0 90 0 1
symversion=0.1
T 46100 44100 5 10 1 1 0 0 1
value=10uF
T 46100 44100 5 10 0 0 0 0 1
footprint=ACY200
}
C 47400 47300 1 0 0 coil-1.sym
{
T 47600 47700 5 10 0 0 0 0 1
device=COIL
T 47600 47500 5 10 1 1 0 0 1
refdes=L101
T 47600 47900 5 10 0 0 0 0 1
symversion=0.1
T 47500 47100 5 10 1 1 0 0 1
value=1000uH
T 47400 47300 5 10 0 0 0 0 1
footprint=RCY300
}
C 52800 47300 1 0 0 vcc-2.sym
{
T 53100 47600 5 10 1 0 0 0 1
net=VCC15:1
}
C 52800 45000 1 270 0 capacitor-2.sym
{
T 53500 44800 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 53300 44500 5 10 1 1 0 0 1
refdes=C102
T 53700 44800 5 10 0 0 270 0 1
symversion=0.1
T 52800 45000 5 10 0 0 0 0 1
footprint=RADIAL_CAN 200
T 53300 44300 5 10 1 1 0 0 1
value=100uF
}
C 46700 45300 1 0 0 njm2360ad.sym
{
T 49000 46300 5 10 0 0 0 0 1
device=NJM2360AD
T 49000 46700 5 10 0 0 0 0 1
footprint=DIP8
T 47300 46800 5 10 1 1 0 0 1
refdes=U101
}
C 45800 43500 1 0 0 gnd-1.sym
C 51600 44000 1 90 0 resistor-1.sym
{
T 51200 44300 5 10 0 0 90 0 1
device=RESISTOR
T 51400 44100 5 10 1 1 90 0 1
refdes=R102
T 51600 44000 5 10 0 0 90 0 1
footprint=R025
T 51800 44100 5 10 1 1 90 0 1
value=9.1K
}
C 51600 45600 1 90 0 resistor-1.sym
{
T 51200 45900 5 10 0 0 90 0 1
device=RESISTOR
T 51400 45700 5 10 1 1 90 0 1
refdes=R103
T 51600 45600 5 10 0 0 90 0 1
footprint=R025
T 51800 45700 5 10 1 1 90 0 1
value=68K
}
C 48800 44100 1 90 0 capacitor-1.sym
{
T 48100 44300 5 10 0 0 90 0 1
device=CAPACITOR
T 48300 44300 5 10 1 1 90 0 1
refdes=C103
T 47900 44300 5 10 0 0 90 0 1
symversion=0.1
T 48900 44100 5 10 1 1 90 0 1
value=1nF
T 48800 44100 5 10 0 0 0 0 1
footprint=ACY100
}
C 46800 46400 1 90 0 resistor-1.sym
{
T 46400 46700 5 10 0 0 90 0 1
device=RESISTOR
T 46600 46500 5 10 1 1 90 0 1
refdes=R104
T 46800 46400 5 10 0 0 90 0 1
footprint=R025
T 47000 46500 5 10 1 1 90 0 1
value=180
}
C 46900 43500 1 0 0 gnd-1.sym
C 48500 43500 1 0 0 gnd-1.sym
C 49400 43500 1 0 0 gnd-1.sym
C 51400 43500 1 0 0 gnd-1.sym
C 52900 43500 1 0 0 gnd-1.sym
N 51300 47300 53000 47300 4
N 53000 47300 53000 45000 4
N 51500 46500 51500 47300 4
N 53000 44100 53000 43800 4
N 51500 44900 51500 45600 4
N 51500 43800 51500 44000 4
N 49900 46100 48600 46100 4
N 49500 44900 49500 46100 4
N 51500 45200 49800 45200 4
N 49800 45200 49800 45800 4
N 49800 45800 48600 45800 4
N 49500 44000 49500 43800 4
N 48600 44100 48600 43800 4
N 47000 43800 47000 45500 4
N 47000 45500 47100 45500 4
N 45900 44100 45900 43800 4
N 45900 45000 45900 47300 4
N 44800 47300 47400 47300 4
N 46700 46400 47100 46400 4
N 47100 46100 45900 46100 4
N 47100 45800 45900 45800 4
N 48600 46400 48600 47300 4
N 48400 47300 50400 47300 4
N 50400 46600 50400 47300 4
C 50300 43500 1 0 0 gnd-1.sym
N 50400 45600 50400 43800 4
C 45700 47300 1 0 0 vcc-2.sym
{
T 46000 47600 5 10 1 0 0 0 1
net=VCC5:1
}
N 48600 45500 48600 45000 4
C 43900 47400 1 180 1 pwrjack-1.sym
{
T 44000 46900 5 10 0 0 180 6 1
device=PWRJACK
T 43900 46900 5 10 1 1 180 6 1
refdes=CONN101
T 43900 47400 5 10 0 0 180 6 1
footprint=PWRJACK_2.1_TO
}
N 45000 47100 45000 43800 4
C 44900 43500 1 0 0 gnd-1.sym
T 50200 40900 9 10 1 0 0 0 1
Septimus    5V to 12V Step-up power supply
T 50400 40200 9 10 1 0 0 0 1
2
C 51300 47500 1 180 0 diode-3.sym
{
T 50850 46950 5 10 0 0 180 0 1
device=DIODE
T 51050 47750 5 10 1 1 180 0 1
refdes=D101
T 51200 47000 5 10 1 1 180 0 1
value=1N4001
T 51300 47500 5 10 0 1 180 0 1
footprint=ALF400
}
T 52100 40200 9 10 1 0 0 0 1
4
N 45000 47100 44800 47100 4
T 53800 40400 9 10 1 0 0 0 1
v0.2
T 53900 40100 9 10 1 0 0 0 1
Robin Scheibler aka FakuFaku