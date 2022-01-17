void main()
{//
    if(openborvariant("in_menuscreen")==1 || openborvariant("in_titlescreen")){
      setglobalvar("F1", NULL());
      setglobalvar("F2", NULL());
      setglobalvar("F3", NULL());
      setglobalvar("F4", NULL());
      setglobalvar("Kalah1", NULL());
      setglobalvar("Kalah2", NULL());
      setglobalvar("Kalah3", NULL());
      setglobalvar("Noma", NULL());

      setglobalvar("C1", 0);

// Random starting seed from msMalik681
      int seed; //initialize seed.
      if(seed==NULL()){
        seed=1;
      } //start it off.
      seed++; //increment seed.
      srand(seed); //apply seed to random function.

    } else if(openborvariant("in_level")==1){
      int K1 = getglobalvar("Kalah1");
      int K2 = getglobalvar("Kalah2");
      int K3 = getglobalvar("Kalah3");

//      drawstring(100,100,8, K1);
//      drawstring(100,120,9, K2);
//      drawstring(100,140,3, K3);
    }
}

void oncreate()
{
    setindexedvar(1, 0);

    void Loading1; void Loading2; void Loading3; void Loading4; void Loading5; 
    
    Loading1 = loadsprite("data/bgs/loading/Loading1.png");
    Loading2 = loadsprite("data/bgs/loading/Loading2.png");
    Loading3 = loadsprite("data/bgs/loading/Loading3.png");
    Loading4 = loadsprite("data/bgs/loading/Loading4.png");
    Loading5 = loadsprite("data/bgs/loading/Loading5.png");

    setglobalvar("Loading1", Loading1);
    setglobalvar("Loading2", Loading2);
    setglobalvar("Loading3", Loading3);
    setglobalvar("Loading4", Loading4);
    setglobalvar("Loading5", Loading5);
}

void ondestroy(){
    void Loading1 = getglobalvar("Loading1");
    void Loading2 = getglobalvar("Loading2");
    void Loading3 = getglobalvar("Loading3");
    void Loading4 = getglobalvar("Loading4");
    void Loading5 = getglobalvar("Loading5");

    free(Loading1);
    free(Loading2);
    free(Loading3);
    free(Loading4);
    free(Loading5);

    setglobalvar("Loading1", NULL());
    setglobalvar("Loading2", NULL());
    setglobalvar("Loading3", NULL());
    setglobalvar("Loading4", NULL());
    setglobalvar("Loading5", NULL());
}