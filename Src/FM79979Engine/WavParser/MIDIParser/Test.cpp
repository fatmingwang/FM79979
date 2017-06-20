#include "stdafx.h"
//Test.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
//#include "MidiFile.h"
//#include "Options.h"
//#include <iostream>
//#include <iomanip>
//
//using namespace std;
////https://stackoverflow.com/questions/2038313/midi-ticks-to-actual-playback-seconds-midi-music
////http://computermusicresource.com/Keyboard.Notes.html
//int main(int argc, char** argv) {
//	Options options;
//	options.process(argc, argv);
//	MidiFile midifile;
//	if (1) {
//		//if (options.getArgCount() > 0) {
//			//midifile.read(options.getArg(1));
//			midifile.read("C:/Users/leeyo/Desktop/midifile-master/midifile-master/visual-studio/Debug/twinkle_twinkle.mid");
//			//midifile.read("C:/Users/leeyo/Desktop/midifile-master/midifile-master/visual-studio/Debug/monteverdi_libri_dei_madrigali_1_5_(c)icking-archive.mid");
//		//midifile.read("C:/Users/leeyo/Desktop/midifile-master/midifile-master/visual-studio/Debug/gra_esp_2.mid");
//		//midifile.read("C:/Users/leeyo/Desktop/midifile-master/midifile-master/visual-studio/Debug/Program_gen_twinkle.mid");
//
//	}
//	else {
//		midifile.read(cin);
//	}
//
//
//	cout << "TPQ: " << midifile.getTicksPerQuarterNote() << endl;
//	cout << "TRACKS: " << midifile.getTrackCount() << endl;
//
//	midifile.joinTracks();
//	// midifile.getTrackCount() will now return "1", but original
//	// track assignments can be seen in .track field of MidiEvent.
//
//	cout << "TICK    \tDELTA   \tTRACK   MIDI MESSAGE\n";
//	cout << "____________________________________\n";
//	if (1)
//	{
//		MidiEvent* mev;
//		int deltatick;
//		int tracks = midifile.getTrackCount();
//		bool l_bNewLine = false;
//		for (int l_iTrack = 0; l_iTrack < tracks; ++l_iTrack)
//		{
//			MidiEventList* l_pMidiEventList = &midifile[l_iTrack];
//			for (int event = 0; event < midifile[l_iTrack].size(); event++)
//			{
//				if (l_bNewLine)
//				{
//					printf("\n");
//					l_bNewLine = false;
//				}
//				mev = &(*l_pMidiEventList)[event];
//				if (event == 0) {
//					deltatick = mev->tick;
//				}
//				else {
//					deltatick = mev->tick - midifile[l_iTrack][event - 1].tick;
//				}
//				if (mev->getKeyNumber() != -1 && mev->isNoteOff())
//				{
//					if (mev->size() == 0)
//					{
//						printf(".");
//						l_bNewLine = true;
//						continue;
//					}
//					double l_dbStartTime = midifile.getTimeInSeconds(mev->tick);
//					double l_fDuration = midifile.getTimeInSeconds(deltatick);
//					printf("%.1lf\t", l_dbStartTime);
//					printf("%.1lf ", l_fDuration);
//					cout << '\t' << mev->track;
//					cout << '\t';
//					printf("%.3d  \n", mev->getKeyNumber());
//				}
//			}
//		}
//	}
//	else
//	{
//	//=======================================================
//		int tracks = midifile.getTrackCount();
//		cout << "TPQ: " << midifile.getTicksPerQuarterNote() << endl;
//		if (tracks > 1)
//		{
//			cout << "TRACKS: " << tracks << endl;
//		}
//		bool l_bNewLine = false;
//		for (int track = 0; track < tracks; track++)
//		{
//			MidiEventList* l_pMidiEventList = &midifile[track];
//			cout << "\nTrack " << track << endl;
//			int deltatick;
//			for (int event = 0; event < midifile[track].size(); event++)
//			{
//				if (l_bNewLine)
//				{
//					printf("\n");
//					l_bNewLine = false;
//				}
//				//================================
//				MidiEvent* l_pMidiEvent = &(*l_pMidiEventList)[event];
//				int l_iSize = l_pMidiEvent->size();
//				int l_iKeyNumber = l_pMidiEvent->getKeyNumber();
//				if (l_iSize == 0 || l_iKeyNumber == -1)
//				{
//					printf(".");
//					l_bNewLine = true;
//					continue;
//				}
//				double l_dbStartTime = midifile.getTimeInSeconds(midifile[track][event].tick);
//				printf("%.1lf\t", l_dbStartTime);
//				if (event == 0)
//				{
//					deltatick = l_pMidiEvent->tick;
//				}
//				else
//				{
//					deltatick = l_pMidiEvent->tick - midifile[track][event - 1].tick;
//				}
//				double l_dbKeyDuration = midifile.getTimeInSeconds(deltatick);
//				printf("%.3d:", l_iKeyNumber);
//				printf("%.2lf ", l_dbKeyDuration);
//				printf("\n");
//			}
//		}
//	}
//	//============================================================
//	//MidiFile outputfile;        // create an empty MIDI file with one track
//	//outputfile.absoluteTicks();  // time information stored as absolute time
//	//							 // (will be coverted to delta time when written)
//	//outputfile.addTrack(2);     // Add another two tracks to the MIDI file
//	//vector<uchar> midievent;     // temporary storage for MIDI events
//	//midievent.resize(3);        // set the size of the array to 3 bytes
//	//int tpq = 120;              // default value in MIDI file is 48
//	//outputfile.setTicksPerQuarterNote(tpq);
//
//	//// data to write to MIDI file: (60 = middle C)
//	//// C5 C  G G A A G-  F F  E  E  D D C-
//	//int melody[50] = { 72,72,79,79,81,81,79,77,77,76,76,74,74,72,-1 };
//	//int mrhythm[50] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,-1 };
//
//	//// C3 C4 E C F C E C D B3 C4 A3 F G C-
//	//int bass[50] = { 48,60,64,60,65,60,64,60,62,59,60,57,53,55,48,-1 };
//	//int brhythm[50] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,-1 };
//
//
//	//// store a melody line in track 1 (track 0 left empty for conductor info)
//	//int i = 0;
//	//int actiontime = 0;      // temporary storage for MIDI event time
//	//midievent[2] = 64;       // store attack/release velocity for note command
//	//while (melody[i] >= 0) {
//	//	midievent[0] = 0x90;     // store a note on command (MIDI channel 1)
//	//	midievent[1] = melody[i];
//	//	outputfile.addEvent(1, actiontime, midievent);
//	//	actiontime += tpq * mrhythm[i];
//	//	midievent[0] = 0x80;     // store a note on command (MIDI channel 1)
//	//	outputfile.addEvent(1, actiontime, midievent);
//	//	i++;
//	//}
//
//	//// store a base line in track 2
//	//i = 0;
//	//actiontime = 0;          // reset time for beginning of file
//	//midievent[2] = 64;
//	//while (bass[i] >= 0) {
//	//	midievent[0] = 0x90;
//	//	midievent[1] = bass[i];
//	//	outputfile.addEvent(2, actiontime, midievent);
//	//	actiontime += tpq * brhythm[i];
//	//	midievent[0] = 0x80;
//	//	outputfile.addEvent(2, actiontime, midievent);
//	//	i++;
//	//}
//
//	//outputfile.sortTracks();         // make sure data is in correct order
//	//outputfile.write("twinkle.mid"); // write Standard MIDI File twinkle.mid
//	printf("\nEnd:\n");
//	char l;
//	cin >> l;
//	return 0;
//}