# LEDmat

Based upon the upstream repo, this is my attempt at a working display.

There is/was decent documentation of how to use the code and setup the hardware, but the code itself doesn't have a ton of commments explaining things.

The first issue I came across was this:
 
    while(1)
     
      {
       temp=pulseIn(serial_in, LOW);
       if(temp>56000 and temp<66000)
         break;
      }

It's looping until it takes between 56,000 and 66,000 microseconds for the data pin coming in from the stackmat timer to go from LOW to HIGH.
At idle, it rarely tops 28,000, so it takes repeated mashing of buttons on the timer to slow it down a bit.
No clue why that's needed, but it works sporadically withouth it, and rarely works with it.

There's also a great description of the protocol [in this reddit post](https://www.reddit.com/r/Cubers/comments/64czya/wip_stackmat_timer_support_for_twistytimer_app/dg19s4y/?utm_term=29267765170&context=3&utm_medium=comment_embed&utm_source=embed&utm_name=4444636c-5b2b-4f6e-8a48-3f5bc6cc87bc) that goes over the protocol itself.

I attempted to have the arduino dump the packets it's collecting to serial, but from the data-dumps I collected, I'm not seeing well-defined bytes with Start 1's and End 0's.  It's all over the place.

I'll keep this updated with progress, but as it stands I can get maybe 1 in 15 "solves" on the timer to actually come across to the display.

There's also an existing body of this having been implemented in [Java](http://timhabermaas.github.io/stackmat.js/) as well as in a [web-interface](https://github.com/search?q=cstimer.net) for the website cstimer.net


## References

- MD_Parola library documentation: https://majicdesigns.github.io/MD_Parola/class_m_d___parola.html#a45d97a582ca1adabfe5cb40d66c4bbd2
- MD_MAX72XX library documentation: https://majicdesigns.github.io/MD_MAX72XX/class_m_d___m_a_x72_x_x.html#a88ea7aada207c02282d091b7be7084e6ab1adfbd7e43930ccfc2317a62447d9f9

