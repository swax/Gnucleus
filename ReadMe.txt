
Welcome to Gnucleus.

This is the interface side of the project, the network layer
of the project is in a COM component called GnucCOM which wraps
the static library GnucDNA.  If you have any questions feel free 
to contact other developers in the ChangeLog file.

Please comment your code and please format your code similar 
to the rest of the project. Any changes made please add to the
ChangeLog file with your name and email address.

 
Buffer Overrun
Check for buffer size and actual buffer size was not the same
should link them together with a define, caused memcpy to overrun
return address of function and signal an error