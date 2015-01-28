import sys
import time
import tweepy
import serial

# Old Spice Spray
consumer_key = CONSUMER_KEY
consumer_secret = CONSUMER_SECRET

# @hezhao
access_key = ACCESS_KEY
access_secret = ACCESS_SECRET 

auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
auth.set_access_token(access_key, access_secret)
api = tweepy.API(auth)

# init serial port
# ser = serial.Serial('/dev/tty.usbmodem1a12321', 9600)
ser = serial.Serial('/dev/tty.usbserial-A400ATUD', 9600)

# IMPORTANT: openning serial port will restart arduino, wait for it
time.sleep(3)

# while True:
#     ser.write('Once a user has logged in, they can then log out or log off when access is no longer needed. To log out is to close off ones access to a computer system after having previously logged in.')
#     time.sleep(5)
#     ser.write('aaa')
#     time.sleep(5)
#     ser.write('bbb')
#     time.sleep(5)


class CustomStreamListener(tweepy.streaming.StreamListener):
    def on_status(self, status):
        if len(status.text):
            msg = '@' + status.user.screen_name.encode('utf-8') + ' ' + status.text.encode('utf-8')
            print msg
            ser.write(msg)
        return True

    def on_error(self, status_code):
        print >> sys.stderr, 'Encountered error with status code:', status_code
        return True # Don't kill the stream

    def on_timeout(self):
        print >> sys.stderr, 'Timeout...'
        return True # Don't kill the stream

stream = tweepy.Stream(auth, CustomStreamListener())
tweets = stream.userstream()
