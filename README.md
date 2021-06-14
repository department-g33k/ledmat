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

# 6/13/2021 Update
I ran the code with Tod's modifications.  As far as I can tell, that PulseIn() nonsense is just to wait until the timer turns on, but I have to "busy up" the timer with save sequences to get the pulse to slow down enough to pass that loop.  I'm leaving it in because I can't be sure it's not required, but it just seems...weird.

```sample_output.txt``` contains a bunch of lines that were dumped by ```Serial.print(String(arr[k]))``` but I find it odd that they vary in length, most are 90 bits, but others are 85.

Finally, I added lines to dump ```packet``` and ```charbuf``` to the console when it successfully reads a time, so we'd have some "good output" to look through.  ```good_output.txt``` contains those dumps.
Curiously, one line is missing an "r" that I thought was a typo on my part, but the typo doesn't exist anywhere in the code, so it almost seems like the arduino's own USB-Serial port dropped a character.

Last night, I also noticed that I got a time of 3.108 on the timer itself, but the display read 8:3.10, in essence the trailing digit was read as the leading digit.  All of that makes me wonder if there isn't a timing issue between the "packets" being read, and how they're being parsed out.

Thanks for all the insight.

## References

- MD_Parola library documentation: https://majicdesigns.github.io/MD_Parola/class_m_d___parola.html#a45d97a582ca1adabfe5cb40d66c4bbd2
- MD_MAX72XX library documentation: https://majicdesigns.github.io/MD_MAX72XX/class_m_d___m_a_x72_x_x.html#a88ea7aada207c02282d091b7be7084e6ab1adfbd7e43930ccfc2317a62447d9f9

