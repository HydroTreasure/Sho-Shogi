# Sho-Shogi
An SDL Program for playing Sho Shogi (no AI support yet)
License: Apache License 2.0 with Restriction for Redistribution of the Work itself (MIT style)  
Notes:  
1. Along with this repository, for proper running, the fond Noto Serif CJK JP Bold is needed under the name and path `\res\NotoSerifCJKjp-Bold.ttf`.
2. Furthermore, since this uses the CPU renderer of SDL as well as the really slow ImGui, this application is somewhat slow when it comes to piece promotion, so please wait for the promotion dialog on moving a piece into / inside the promotion zone and do not hurry up while playing the game.
3. There may exist certain bugs. One known bug is about a not so important text not being rendered at the end of the game. Another bug is that you cannot promote a piece while exiting the promotion zone. There may be any more unknown bugs.
4. There is no AI in this application.
5. This application does not show all valid moves, even though there is move validation.
