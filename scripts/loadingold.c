void main()
{//Custom loading screen using sprite animations
	int max			= getlocalvar("max");
	int val			= getlocalvar("value");
	int loadFrame	= 0;
	int startIndex	= 0;
	int endIndex;

	if(openborvariant("in_level")){
		if(val < max){
			if(getlocalvar("frame") == NULL()){setlocalvar("frame", startIndex);}
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0001.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0002.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0003.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0004.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0005.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0006.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0007.png"));}
			loadFrame = loadFrame+1;
			if(getglobalvar(""+loadFrame) == NULL()){setglobalvar(""+loadFrame, loadsprite("data/sprites/loading/loading_frame_0008.png"));}
			endIndex = loadFrame;

			drawsprite(getglobalvar(""+getlocalvar("frame")), 0, 0, 0);

			if(getlocalvar("frame") < endIndex){
				setlocalvar("frame", getlocalvar("frame")+1);
			}
			else
			{
				setlocalvar("frame", startIndex);
			}
		}
		else
		{
			clearlocalvar();
		}
	}
}