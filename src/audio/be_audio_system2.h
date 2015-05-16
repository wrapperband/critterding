#ifndef BE_AUDIO_SYSTEM2_H_INCLUDED
#define BE_AUDIO_SYSTEM2_H_INCLUDED

#ifdef HAVE_OPENAL

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <AL/al.h> 
#include <AL/alc.h> 
#include <sys/time.h>
#include <ctime> 
// #include "audio/be_audio_system2.h"

class BeAudioSystem
{
	public:
		BeAudioSystem()
		{
			// Print the list of capture devices 
				printf("Available playback devices:\n");

				devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER); 
				ptr = devices; 
				//while (ptr[0] != NULL)
				while (*ptr)
				{ 
					printf("   %s\n", ptr); 
					ptr += strlen(ptr) + 1; 
				} 

			// Open a playback device and create a context first 
				printf("Opening playback device:\n"); 
				mainDev = alcOpenDevice(NULL); 
				if (mainDev == NULL) 
				{ 
					printf("Unable to open playback device!\n"); 
					exit(1); 
				} 
				devices = alcGetString(mainDev, ALC_DEVICE_SPECIFIER); 
				printf("   opened device '%s'\n", devices); 
				mainContext = alcCreateContext(mainDev, NULL); 
				if (mainContext == NULL) 
				{ 
					printf("Unable to create playback context!\n"); 
					exit(1); 
				} 
				printf("   created playback context\n"); 

			// Make the playback context current 
				alcMakeContextCurrent(mainContext); 
				alcProcessContext(mainContext); 

			// Print the list of capture devices 
				printf("Available capture devices:\n"); 
				devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
				ptr = devices; 

				//while (ptr[0] != NULL)
				while (*ptr)
				{ 
					printf("   %s\n", ptr); 
					ptr += strlen(ptr) + 1; 
				}

			// Open the default device 
				printf("Opening capture device:\n"); 
				captureDev = alcCaptureOpenDevice(NULL, 8000, AL_FORMAT_MONO16, 800); 
				if (captureDev == NULL) 
				{  
					printf("   Unable to open device!\n"); 
					exit(1); 
				} 
				devices = alcGetString(captureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
				printf("   opened device %s\n", devices); 
				
			// Wait for three seconds to prompt the user 
				for (i = 3; i > 0; i--) 
				{ 
					printf("Starting capture in %d...\r", i); 
					fflush(stdout); 
					lastTime = time(NULL); 
					currentTime = lastTime; 
					while (currentTime == lastTime) 
					{ 
						currentTime = time(NULL); 
						usleep(100000); 
					} 
				} 

				printf("Starting capture NOW!\n"); 
				fflush(stdout); 
				lastTime = currentTime; 
		}
		
		void process( /*const BeTimer& timer*/ )
		{



			// Capture (roughly) five seconds of audio 
				alcCaptureStart(captureDev); 
				samplesCaptured = 0; 
				captureBufPtr = captureBuffer; 
				while (currentTime < (lastTime + 5)) 
				{ 
					// Get the number of samples available 
					alcGetIntegerv(captureDev, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable); 

					// Copy the samples to our capture buffer 
					if (samplesAvailable > 0) 
					{ 
						alcCaptureSamples(captureDev, captureBufPtr, samplesAvailable); 
						samplesCaptured += samplesAvailable; 
						printf("Captured %d samples (adding %d)\r", samplesCaptured, 
							samplesAvailable); 
						fflush(stdout); 

						// Advance the buffer (two bytes per sample * number of samples) 
						captureBufPtr += samplesAvailable * 2; 
					} 

					// Wait for a bit 
					usleep(10000); 

					// Update the clock 
					currentTime = time(NULL); 
				} 
				printf("\nPausing capture.\n"); 
				alcCaptureStop(captureDev); 

			// Wait for three seconds to prompt the user 
				for (i = 3; i > 0; i--) 
				{ 
					printf("Resuming capture in %d...\r", i); 
					fflush(stdout); 
					lastTime = time(NULL); 
					currentTime = lastTime; 
					while (currentTime == lastTime) 
					{ 
						currentTime = time(NULL); 
						usleep(100000); 
					} 
				} 

				printf("Resuming capture NOW!\n"); 
				fflush(stdout); 
				lastTime = currentTime; 

			// Capture (roughly) five seconds of audio 
				alcCaptureStart(captureDev); 
				while (currentTime < (lastTime + 5)) 
				{ 
					// Get the number of samples available 
					alcGetIntegerv(captureDev, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable); 

					// Copy the samples to our capture buffer 
					if (samplesAvailable > 0) 
					{ 
						alcCaptureSamples(captureDev, captureBufPtr, samplesAvailable); 
						samplesCaptured += samplesAvailable; 
						printf("Captured %d samples (adding %d)\r", samplesCaptured, 
							samplesAvailable); 
						fflush(stdout); 

						// Advance the buffer (two bytes per sample * number of samples) 
						captureBufPtr += samplesAvailable * 2; 
					} 

					// Wait for a bit 
					usleep(10000); 

					// Update the clock 
					currentTime = time(NULL); 
				} 

				printf("\nDone capturing.\n"); 
				alcCaptureStop(captureDev); 

			// Play back the captured data 
				printf("Starting playback...\n"); 
				fflush(stdout); 

			// Generate an OpenAL buffer for the captured data 
				alGenBuffers(1, &buffer); 
				alGenSources(1, &source); 
				alBufferData(buffer, AL_FORMAT_MONO16, captureBuffer,samplesCaptured*2, 8000); 
				alSourcei(source, AL_BUFFER, buffer); 
				alSourcePlay(source); 

			// Wait for the source to stop playing 
				playState = AL_PLAYING; 
				while (playState == AL_PLAYING) 
				{ 
					printf("  source %d is playing...\r", source); 
					fflush(stdout); 
					alGetSourcei(source, AL_SOURCE_STATE, &playState); 
					usleep(100000); 
				} 
				printf("\nDone with playback.\n"); 
				fflush(stdout);
						
		}
		
		~BeAudioSystem()
		{
			// Shut down OpenAL 
			alDeleteSources(1, &source); 
			alDeleteBuffers(1, &buffer); 
			alcMakeContextCurrent(NULL); 
			alcCloseDevice(mainDev); 
			alcCaptureCloseDevice(captureDev); 
		};
	private:
		const ALCchar *   devices; 
		const ALCchar *         ptr; 
		ALCdevice *       mainDev; 
		ALCcontext *      mainContext; 
		ALCdevice *       captureDev; 
		ALubyte           captureBuffer[1048576]; 
		ALubyte           *captureBufPtr; 
		ALint             samplesAvailable; 
		ALint             samplesCaptured; 
		time_t            currentTime; 
		time_t            lastTime; 
		ALuint            buffer; 
		ALuint            source; 
		ALint             playState; 
		int               i; 
};


#endif
#endif
