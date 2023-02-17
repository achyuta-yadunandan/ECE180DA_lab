#ask for user input 
#have ai pick random move
#calculae winners
# print out winter
# ask for next user input 
import random
keep_play=True
while keep_play:
    user_pick= input("Enter a option: rock, paper, scissors: ")
    print(user_pick)
    options = "rock", "paper", "scissors"
    ai_pick=random.choice(options)
    print("ai picked", ai_pick)

    if(user_pick=="rock"):
        if(ai_pick=="scissors"):
            output="user wins"
        if(ai_pick=="rock"):
            output="tie"
        if(ai_pick=="paper"):
            output="ai wins"

    if(user_pick=="scissors"):
        if(ai_pick=="scissors"):
            output="tie"
        if(ai_pick=="rock"):
            output="ai wins"
        if(ai_pick=="paper"):
            output="user wins"

    if(user_pick=="paper"):
        if(ai_pick=="scissors"):
            output="ai wins"
        if(ai_pick=="rock"):
            output="user wins"
        if(ai_pick=="paper"):
            output="tie"
    print(output)

    end= input("Play again? (yes/no) ")

    if(end=="no"):
        keep_play=False

    
