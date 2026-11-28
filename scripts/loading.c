void main()
{
    int value = getlocalvar("value");
    int max = getlocalvar("max");
    int largeur = 0;
    int frame = 0;
    void sprite;

    if(isempty(getglobalvar("ss_loading_frame")))
    {
        setglobalvar("ss_loading_sprite0", loadsprite("data/bgs/loading/frame00.png"));
        setglobalvar("ss_loading_sprite1", loadsprite("data/bgs/loading/frame01.png"));
        setglobalvar("ss_loading_sprite2", loadsprite("data/bgs/loading/frame02.png"));
        setglobalvar("ss_loading_sprite3", loadsprite("data/bgs/loading/frame03.png"));
        setglobalvar("ss_loading_sprite4", loadsprite("data/bgs/loading/frame04.png"));
        setglobalvar("ss_loading_sprite5", loadsprite("data/bgs/loading/frame05.png"));
        setglobalvar("ss_loading_sprite6", loadsprite("data/bgs/loading/frame06.png"));
        setglobalvar("ss_loading_sprite7", loadsprite("data/bgs/loading/frame07.png"));
        setglobalvar("ss_loading_sprite8", loadsprite("data/bgs/loading/frame08.png"));
        setglobalvar("ss_loading_sprite9", loadsprite("data/bgs/loading/frame09.png"));
        setglobalvar("ss_loading_sprite10", loadsprite("data/bgs/loading/frame10.png"));
        setglobalvar("ss_loading_sprite11", loadsprite("data/bgs/loading/frame11.png"));
        setglobalvar("ss_loading_frame", 0);
    }
    frame = getglobalvar("ss_loading_frame");
    if(frame == 0) sprite = getglobalvar("ss_loading_sprite0");
    if(frame == 1) sprite = getglobalvar("ss_loading_sprite1");
    if(frame == 2) sprite = getglobalvar("ss_loading_sprite2");
    if(frame == 3) sprite = getglobalvar("ss_loading_sprite3");
    if(frame == 4) sprite = getglobalvar("ss_loading_sprite4");
    if(frame == 5) sprite = getglobalvar("ss_loading_sprite5");
    if(frame == 6) sprite = getglobalvar("ss_loading_sprite6");
    if(frame == 7) sprite = getglobalvar("ss_loading_sprite7");
    if(frame == 8) sprite = getglobalvar("ss_loading_sprite8");
    if(frame == 9) sprite = getglobalvar("ss_loading_sprite9");
    if(frame == 10) sprite = getglobalvar("ss_loading_sprite10");
    if(frame == 11) sprite = getglobalvar("ss_loading_sprite11");
    drawbox(0, 0, 864, 484, 999998, rgbcolor(0, 0, 0));
    drawsprite(sprite, 0, 0, 999999, 0);
    setglobalvar("ss_loading_frame", (frame + 1) % 12);
    if(value >= 0 && max > 0)
    {
        largeur = (value * 480) / max;
        if(largeur > 480) largeur = 480;
    }
    drawbox(208, 443, 480, 12, 1000000, rgbcolor(0, 0, 0));
    if(largeur > 0)
    {
        drawbox(208, 443, largeur, 12, 1000001, rgbcolor(255, 255, 0));
    }
}
