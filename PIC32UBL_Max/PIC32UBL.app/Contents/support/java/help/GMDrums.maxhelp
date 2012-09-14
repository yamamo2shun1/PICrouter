{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 89.0, 74.0, 955.0, 589.0 ],
		"bglocked" : 0,
		"defrect" : [ 89.0, 74.0, 955.0, 589.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"metadata" : [  ],
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "viewsource",
					"fontname" : "Arial",
					"patching_rect" : [ 574.470337, 348.0, 78.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-1",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"linecount" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 731.436584, 34.0, 50.0, 34.0 ],
					"hidden" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-2",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.75 0.25 0.5 0.125 0.125 0.25 0.5",
					"fontname" : "Arial",
					"patching_rect" : [ 751.50177, 151.0, 194.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-3",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj MetroSeq @interval 300",
					"fontname" : "Arial",
					"patching_rect" : [ 685.599548, 191.0, 185.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"id" : "obj-4",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.5 0.25 0.5 0.25 0.25 0.25",
					"fontname" : "Arial",
					"patching_rect" : [ 618.405151, 126.0, 181.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-5",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj MetroSeq @interval 300",
					"fontname" : "Arial",
					"patching_rect" : [ 553.795105, 157.0, 185.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"id" : "obj-6",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"patching_rect" : [ 197.147858, 74.0, 19.0, 68.0 ],
					"orientation" : 2,
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-7",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 197.147858, 51.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 50.0, 20.0 ],
					"id" : "obj-8",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 1 1 1 3 3 3 1 2 2 0.5 1 0.5",
					"fontname" : "Arial",
					"patching_rect" : [ 619.697327, 80.0, 188.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-9",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"patching_rect" : [ 485.308472, 65.0, 39.0, 39.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-10",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj MetroSeq @interval 300",
					"fontname" : "Arial",
					"patching_rect" : [ 525.366699, 108.0, 185.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"id" : "obj-11",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "start/stop",
					"fontname" : "Arial",
					"patching_rect" : [ 542.165283, 84.0, 70.0, 20.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-12"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend voice",
					"fontname" : "Arial",
					"patching_rect" : [ 322.491272, 309.0, 92.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-13",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dumpvoicenames",
					"fontname" : "Arial",
					"patching_rect" : [ 178.862625, 395.0, 110.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-14",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"fontname" : "Arial",
					"patching_rect" : [ 206.193237, 284.0, 100.0, 20.0 ],
					"types" : [  ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"items" : [ "ACOUSTIC_BASS_DRUM", ",", "BASS_DRUM", ",", "SIDE_STICK", ",", "ACOUSTIC_SNARE", ",", "HAND_CLAP", ",", "ELECTRIC_SNARE", ",", "LOW_FLOOR_TOM", ",", "CLOSED_HI_HAT", ",", "HIGH_FLOOR_TOM", ",", "PEDAL_HI_HAT", ",", "LOW_TOM", ",", "OPEN_HI_HAT", ",", "LOW_MID_TOM", ",", "HI_MID_TOM", ",", "CRASH_CYMBAL_1", ",", "HIGH_TOM", ",", "RIDE_CYMBAL_1", ",", "CHINESE_CYMBAL", ",", "RIDE_BELL", ",", "TAMBOURINE", ",", "SPLASH_CYMBAL", ",", "COWBELL", ",", "CRASH_CYMBAL_2", ",", "VIBRASLAP", ",", "RIDE_CYMBAL_2", ",", "HI_BONGO", ",", "LOW_BONGO", ",", "MUTE_HI_CONGA", ",", "OPEN_HI_CONGA", ",", "LOW_CONGA", ",", "HIGH_TIMBALE", ",", "LOW_TIMBALE", ",", "HIGH_AGOGO", ",", "LOW_AGOGO", ",", "CABASA", ",", "MARACAS", ",", "SHORT_WHISTLE", ",", "LONG_WHISTLE", ",", "SHORT_GUIRO", ",", "LONG_GUIRO", ",", "CLAVES", ",", "HI_WOOD_BLOCK", ",", "LOW_WOOD_BLOCK", ",", "MUTE_CUICA", ",", "OPEN_CUICA", ",", "MUTE_TRIANGLE", ",", "OPEN_TRIANGLE" ],
					"numoutlets" : 3,
					"labelclick" : 1,
					"id" : "obj-15",
					"outlettype" : [ "int", "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 350.919678, 168.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"minimum" : 35,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 50.0, 20.0 ],
					"maximum" : 81,
					"id" : "obj-16",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 260.465668, 168.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"minimum" : 35,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 50.0, 20.0 ],
					"maximum" : 81,
					"id" : "obj-17",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 166.13504, 170.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"minimum" : 27,
					"numoutlets" : 2,
					"presentation_rect" : [ 0.0, 0.0, 50.0, 20.0 ],
					"maximum" : 127,
					"id" : "obj-18",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "voice $1",
					"fontname" : "Arial",
					"patching_rect" : [ 350.919678, 193.0, 61.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-19",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "voice $1",
					"fontname" : "Arial",
					"patching_rect" : [ 260.465668, 193.0, 61.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-20",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "voice $1",
					"fontname" : "Arial",
					"patching_rect" : [ 166.13504, 193.0, 61.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-21",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 418.114075, 218.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-22",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 275.972046, 217.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-23",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 206.193237, 217.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-24",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj GMDrums 81 36 PEDAL_HI_HAT",
					"fontname" : "Arial",
					"patching_rect" : [ 206.193237, 251.0, 231.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 3,
					"numoutlets" : 1,
					"id" : "obj-25",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "ACOUSTIC_BASS_DRUM BASS_DRUM \rSIDE_STICK\rACOUSTIC_SNARE\rHAND_CLAP \rELECTRIC_SNARE LOW_FLOOR_TOM  CLOSED_HI_HAT  HIGH_FLOOR_TOM  PEDAL_HI_HAT  \rLOW_TOM \rOPEN_HI_HAT \rLOW_MID_TOM \rHI_MID_TOM \rCRASH_CYMBAL_1 \rHIGH_TOM \rRIDE_CYMBAL_1 CHINESE_CYMBAL \rRIDE_BELL  \rTAMBOURINE \rSPLASH_CYMBAL  \rCOWBELL  \rCRASH_CYMBAL_2 \rVIBRASLAP  \rRIDE_CYMBAL_2  \rHI_BONGO  \rLOW_BONGO  \rMUTE_HI_CONGA  OPEN_HI_CONGA  \rLOW_CONGA  \rHIGH_TIMBALE \rLOW_TIMBALE  \rHIGH_AGOGO  \rLOW_AGOGO  \rCABASA  \rMARACAS  \rSHORT_WHISTLE  LONG_WHISTLE \rSHORT_GUIRO  \rLONG_GUIRO  \rCLAVES  \rHI_WOOD_BLOCK  LOW_WOOD_BLOCK \rMUTE_CUICA  \rOPEN_CUICA \rMUTE_TRIANGLE  OPEN_TRIANGLE ",
					"linecount" : 47,
					"fontname" : "Courier",
					"patching_rect" : [ 0.0, 9.0, 128.0, 570.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-26"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "35\r36\r37\r38\r39\r40\r41\r42\r43\r44\r45\r46\r47\r48\r49\r50\r51\r52\r53\r54\r55\r56\r57\r58\r59\r60\r61\r62\r63\r64\r65\r66\r67\r68\r69\r70\r71\r72\r73\r74\r75\r76\r77\r78\r79\r80\r81\r",
					"linecount" : 47,
					"fontname" : "Courier",
					"patching_rect" : [ 124.051224, 9.0, 33.0, 570.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-27"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dumpvoicenames causes GM drum voice names to be output info outlet in a format suitable for a menu. This message is called automatically on patcher loading.",
					"linecount" : 4,
					"fontname" : "Arial",
					"patching_rect" : [ 190.492432, 415.0, 231.0, 62.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-28"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The [voice ] message changes the voice of the inlet it is sent to.",
					"linecount" : 3,
					"fontname" : "Arial",
					"patching_rect" : [ 418.114075, 303.0, 130.0, 48.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-29"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "A number in an inlet sets the velocity of the notes (0-127)",
					"linecount" : 3,
					"fontname" : "Arial",
					"patching_rect" : [ 229.45285, 74.0, 119.0, 48.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-30"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang on inlet triggers voice",
					"fontname" : "Arial",
					"patching_rect" : [ 440.081482, 216.0, 183.0, 20.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-31"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Init args are GM Drum Map numbers or GM Drum Names. One inlet is created for each voice. Send messages directly to an inlet to affect that voice.",
					"linecount" : 3,
					"fontname" : "Arial",
					"patching_rect" : [ 442.665894, 250.0, 277.0, 48.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-32"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "GMDrums.help",
					"fontname" : "Arial",
					"patching_rect" : [ 595.743896, 416.0, 189.0, 27.0 ],
					"fontsize" : 18.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-33"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "by tml",
					"fontname" : "Arial",
					"patching_rect" : [ 731.424927, 445.0, 125.0, 20.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-34"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "This is provided as an example of the javax.sound API. Your milage may vary as performance is implementation dependent depending on your java platform",
					"linecount" : 4,
					"fontname" : "Arial",
					"patching_rect" : [ 597.036072, 468.0, 240.0, 62.0 ],
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-35"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [ 534.866699, 133.0, 242.374847, 133.0, 242.374847, 214.0, 215.693237, 214.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [ 206.647858, 153.0, 232.037262, 153.0, 232.037262, 245.0, 215.693237, 245.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [ 331.991272, 335.0, 239.790466, 335.0, 239.790466, 319.0, 194.563446, 319.0, 194.563446, 248.0, 215.693237, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [ 175.63504, 237.0, 215.693237, 237.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [ 188.362625, 423.0, 171.356232, 423.0, 171.356232, 247.0, 215.693237, 247.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [ 563.295105, 201.0, 445.0, 201.0, 445.0, 156.0, 335.0, 156.0, 335.0, 213.0, 285.472046, 213.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-25", 1 ],
					"hidden" : 0,
					"midpoints" : [ 269.965668, 238.0, 321.693237, 238.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-25", 1 ],
					"hidden" : 0,
					"midpoints" : [ 285.472046, 241.0, 321.693237, 241.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 695.099548, 215.0, 427.614075, 215.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-25", 2 ],
					"hidden" : 0,
					"midpoints" : [ 360.419678, 239.0, 427.693237, 239.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-25", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 629.197327, 107.0, 534.866699, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 494.808472, 147.0, 563.295105, 147.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 627.905151, 147.0, 563.295105, 147.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 494.808472, 188.0, 695.099548, 188.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 761.00177, 181.0, 695.099548, 181.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
