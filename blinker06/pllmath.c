

#include <stdio.h>

int main ( void )
{
    unsigned int m,d;
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int vco;
    unsigned int min;

    min=0; min--;

    for(d=1;d<=255;d++)
    {
        vco=18432/d;
        //if(vco<1000) continue;
        for(m=1;m<2047;m++)
        {
            vco=(18432*m)/d;
            rb=2;
            {
                ra=55000*rb;
                if(vco<ra) rc=ra-vco; else rc=vco-ra;
                if(rc<min)
                {
                    printf("d %u m %u %u %u\n",d,m,vco,rc);
                    min=rc;
                }
            }
        }
    }
    return(0);
}

