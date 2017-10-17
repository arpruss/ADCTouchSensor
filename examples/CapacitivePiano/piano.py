import pygame.midi
import pygame
import time
import serial
from sys import argv,version_info
from time import sleep

byteToInt = int if version_info[0] >= 3 else ord

noteValues = [60, 62, 64, 65, 67, 69, 71, 72];
noteNames = [ 'C','D','E','F','G','A','B','C'];

width = 800
height = 600
keyWidth = width//len(noteValues)
WHITE = (255,255,255)
BLACK = (0,0,0)

command = [0,0];
commandIndex = 0;

if len(argv) >= 2 and argv[1] == "--no-serial":
    ser = None
else:
    port = "com7" if len(argv)<2 else argv[1]
    print("Connecting to "+port)
    ser = serial.Serial(port, baudrate=115200, timeout=0)
    ser.readline() # skip a potential partial line

def drawKey(i,value):
    x = keyWidth * i
    pygame.draw.rect(screen, BLACK if value else WHITE, [x,0,keyWidth,height], 0)
    pygame.draw.rect(screen, WHITE if value else BLACK, [x,0,keyWidth,height], 2)
    size = font.size(noteNames[i])
    screen.blit(font.render(noteNames[i],1,
        (255,255,255) if value else (0,0,0)), (x+keyWidth/2-size[0]/2,height*3/4-size[1]/2))
    pygame.display.flip()
    
print("MIDI setup")
pygame.midi.init()
midiOut = pygame.midi.Output(0)

print("Screen setup")
pygame.init()
screen = pygame.display.set_mode((width,height))
pygame.display.set_caption("Piano")
font = pygame.font.Font(None,30)

for i in range(len(noteNames)):
    drawKey(i,False)

done = False

mouseNote = None

def playNote(note,velocity,state):
    i = noteValues.index(note)
    if state:
        midiOut.note_on(note, velocity)
        if 0 <= i:
            print(noteNames[i])
            drawKey(i,True)
    else:
        midiOut.note_off(note, velocity)
        if 0 <= i:
            drawKey(i,False)
    
try:
    while not done:
        changed = False
        if ser is not None:
            data = ser.read(1)
            if len(data):
                datum = byteToInt(data[0])
                if datum & 0x80:
                    command[0] = datum
                    commandIndex = 1
                elif commandIndex:
                    if commandIndex == 2:
                       if (command[0] & 0xF0) == 0b10010000:
                           playNote(command[1],datum,True)
                       elif (command[0] & 0xF0) == 0b10000000:
                           playNote(command[1],datum,False)
                       commandIndex = 0
                    else:
                       command[commandIndex] = datum
                       commandIndex += 1
                    
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if mouseNote is not None:
                    playNote(noteValues[mouseNote],127,False)
                mouseNote = event.pos[0] // keyWidth
                if mouseNote < len(noteValues):
                    playNote(noteValues[mouseNote],127,True)
                else:
                    mouseNote = None
            elif event.type == pygame.MOUSEBUTTONUP:
                if mouseNote is not None:
                    playNote(noteValues[mouseNote],127,False)
                    mouseNote = None
                
except serial.SerialException:
    print("Disconnected!")

midiOut.close()
pygame.quit()
