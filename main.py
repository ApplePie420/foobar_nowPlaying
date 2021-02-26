import serial
import settings
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

# start serial communication on port specified in settings.py file and on specified baud rate
# ! Note that baud rate here and in Arduino program has to match, otherwise you'll get corrupted data !
ser = serial.Serial()
ser.baudrate = settings.baud_rate
ser.port = settings.COM_port
ser.open()

# this is out event handler. Whenever a file is modifier, that code snippet will run. It allows us to basically
# "monitor" our file if it has been changed by foobar plugin. If yes, then parse data and yeet them over to Arduino
class EventHandler(FileSystemEventHandler):
    def on_modified(self, event):
        # wait for a second. Sometimes this event is triggered too quickly, and data will be el garbagio. Also, it causes this program
        # to crash, so it is here for a good reason :)
        time.sleep(1)
        #  uncomment if you debug stuff
        #print(event)

        # open the file
        f = open(settings.foobar_file_path + settings.foobar_filename, "r")
        fileContent = f.readlines()
        # if there is only one line (presumably because foobar is closed), print it to display
        # otherwise, assume everything is okay, continue
        if(len(fileContent) == 1):
            time.sleep(1)
            ser.write(b"foobar is closed")
        else:
            # expect artist on first line. Strip "\n" character and encode to standard byte format, so our Arduino can understand it
            artist = fileContent[0].rstrip()
            #print(artist)
            artist = artist.encode("UTF-8")

            # do the same for title, which is expected on second line
            title = fileContent[1].rstrip()
            #print(title)
            title = title.encode("UTF-8")

            # let Arduino wait a bit. This has to be here, because our foobar plugin fires the event twice for some reason. Idk what is it doing
            # with the file, but it triggers it twice. So, we need to wait a bit before sending it to Arduino, otherwise we can get
            # ugly mixed strings
            time.sleep(1)
            # send artist string
            ser.write(artist)
            # wait for Arduino to process
            time.sleep(1)
            # send title string
            ser.write(title)

# create our handler thing
handler = EventHandler()
# same with observer
observer = Observer()
# schedule observer to trigger our EvenetHandler(), on specified file. Since it's file, we don't need to recursively check
observer.schedule(handler, path=settings.foobar_file_path, recursive=False)
# start the observer
observer.start()

# I have no idea what this is, but you can't CTRL+C without it. I guess that the observer thing is calling some deep system stuff and 
# prevents the thread from just killing it. Idk honestly.
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    observer.stop()
    ser.close()
observer.join()