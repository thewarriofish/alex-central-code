<script>
	let widths = [5, 222, 438, 660, 875, 1100, 1313]
</script>

<layout></layout>

<calendar></calendar>



<outline></outline>

<vertical_line style="left: 216px;"></vertical_line>
<vertical_line style="left: 657px;"></vertical_line>
<vertical_line style="left: 1095px;"></vertical_line>

<horizontal_line style="top: 262px;"></horizontal_line>
<horizontal_line style="top: 554px;"></horizontal_line>
<horizontal_line style="top: 840px;"></horizontal_line>



<month_year></month_year>

<selection></selection>

<big_font style=
"left: 35px;
top: 32px;
font-weight: 400;">October</big_font>
<big_font style=
"left: 172px;
top: 32px;
font-weight: 700;">2022</big_font>

<small_font style="left: 0px; top: 49px;">Sunday</small_font>
<small_font style="left: 219px; top: 49px;">Monday</small_font>
<small_font style="left: 435px; top: 49px;">Tuesday</small_font>
<small_font style="left: 655px; top: 49px;">Wednesday</small_font>
<small_font style="left: 870px; top: 49px;">Thursday</small_font>
<small_font style="left: 1095px; top: 49px;">Friday</small_font>
<small_font style="left: 1310px; top: 49px;">Saturday</small_font>

<unimportant_font style="left: 5px; top: 81px;">25</unimportant_font>
<unimportant_font style="left: 225px; top: 81px;">26</unimportant_font>
<unimportant_font style="left: 440px; top: 81px;">27</unimportant_font>
<unimportant_font style="left: 660px; top: 81px;">28</unimportant_font>
<unimportant_font style="left: 875px; top: 81px;">29</unimportant_font>
<unimportant_font style="left: 1100px; top: 81px;">30</unimportant_font>
<small_font style="left: 1313px; top: 81px;">1</small_font>

{#each widths as w, i}
    <small_font style="left: {w}px; top: 212px">{i + 2}</small_font>
{/each}

{#each widths as w, i}
    <small_font style="left: {w}px; top: 354px">{i + 9}</small_font>
{/each}

{#each widths as w, i}
    <small_font style="left: {w}px; top: 506px">{i + 16}</small_font>
{/each}

{#each widths as w, i}
    <small_font style="left: {w}px; top: 648px">{i + 23}</small_font>
{/each}

<small_font style="left: 5px; top: 794px;">30</small_font>
<small_font style="left: 222px; top: 794px;">31</small_font>

<unimportant_font style="left: 439px; top: 792px;">1</unimportant_font>
<unimportant_font style="left: 668px; top: 792px;">2</unimportant_font>
<unimportant_font style="left: 879px; top: 792px;">3</unimportant_font>
<unimportant_font style="left: 1103px; top: 792px;">4</unimportant_font>
<unimportant_font style="left: 1317px; top: 792px;">5</unimportant_font>


<style>

@font-face{
    font-family: 'Inter';
    font-style: normal;
    src: local('Inter'), format(woff2);
}
layout{
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    padding: 0px;
    position: relative;
    width: 1512px;
    height: 982px;
    background: #FFC79F;
    box-shadow: 0px 4px 4px rgba(0, 0, 0, 0.25); 
}

calendar{
    width: 1512px;
    height: 982px;
    flex: none;
    order: 0;
    flex-grow: 0;  
}

big_font{
    position: absolute;
    width: 140px;
    height: 26px;
    font-size: 36px;
    line-height: 44px;
    display: flex;
    align-items: center;
    text-align: center;
    color: #000000;
}

small_font{
    position: absolute;
    width: 225px;
    height: 135px;
    font-weight: 400;
    font-size: 32px;
    line-height: 39px;
    display: flex;
    align-items: center;
    text-align: center;
    color: #000000;
}

unimportant_font{
    position: absolute;
    width: 225px;
    height: 135px;
    font-weight: 400;
    font-size: 32px;
    line-height: 39px;
    display: flex;
    align-items: center;
    text-align: center;
    color: #B3B3B3;
}

outline{
    box-sizing: border-box;
    position: absolute;
    width: 1512px;
    height: 852px;
    left: 0px;
    top: 130px;
    border: 1px solid #000000; 
}

vertical_line{
    box-sizing: border-box;
    position: absolute;
    width: 216px;
    height: 852px;
    top: 130px;
    border: 1px solid #000000;
}

horizontal_line{
    box-sizing: border-box;
    position: absolute;
    width: 1512px;
    height: 142px;
    left: 0px;
    border: 1px solid #000000;
}

month_year{
    position: absolute;
    width: 256px;
    height: 42px;
    left: 21px;
    top: 25px;
    background: #FF889C;
    border-radius: 10px;
}

selection{
    position: absolute;
    width: 36px;
    height: 36px;
    left: 438px;
    top: 406px;
    background: #FF889C;
    box-shadow: 0px 4px 4px rgba(0, 0, 0, 0.25);
    border-radius: 10px;
}
</style>