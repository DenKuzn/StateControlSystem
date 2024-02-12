int Main()
{
  Two Systems in my code.
    I have created the base 2 systems that can be used for strategy games.
    
  State Control System.
    System based on layered implementation where each layer represents a new type of control. It is a State Pattern. You can easily create your own new state and connect it to the system. For example, a Base state with mouse interaction. When you click on some unit (settler, tank, car, barrel), a New state will be activated for controlling this unit. This new state will also control all inputs from the player and decide how to behave at the moment. You can even create a TPS or FPS state and switch between strategy and these TPS/FPS genres.
    
  Unit Order System.
    Based on the Command pattern with an Ability System. This Plugin will be useful for tactical games like Jagged Alliance or 7.62. Unfortunately, I still haven't added a good example for using it, but you can read the code, right? =))
    
  P.S. There are a lot of comments in Russian. I will translate them into English if it will be necessary.
}
