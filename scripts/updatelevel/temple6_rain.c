#import "data/scripts/updatelevel/main.c"

void main()
{
	//cycleSprite("layer_type", "first_frame", "last_frame", "delay");
	cycleSprite("fglayer", 1, 8, 12);
	cycleSprite("fglayer", 9, 12, 12);
	cycleSprite("fglayer", 13, 16, 12);
	cycleSprite("fglayer", 17, 20, 12);
}