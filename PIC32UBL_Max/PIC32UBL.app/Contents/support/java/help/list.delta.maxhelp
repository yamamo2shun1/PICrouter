{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 35.0, 54.0, 486.0, 215.0 ],
		"bglocked" : 0,
		"defrect" : [ 35.0, 54.0, 486.0, 215.0 ],
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
					"text" : "listpackage",
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 390.40332, 168.229507, 76.0, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend help",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 390.40332, 191.590164, 83.0, 20.0 ],
					"numoutlets" : 1,
					"hidden" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pcontrol",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 390.40332, 217.409836, 56.0, 20.0 ],
					"numoutlets" : 1,
					"hidden" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print delta",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 29.0, 171.0, 62.171799, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 11.63564,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "I have 5 fingers on each of my 2 hands",
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 265.262817, 100.0, 214.147308, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 11.63564,
					"outlettype" : [ "" ],
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 1 2 3 5 6 74",
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 170.015518, 100.0, 82.895729, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 11.63564,
					"outlettype" : [ "" ],
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "given a list of length N, outputs a list of length N-1 whose elements are the difference between successive elements from the input list",
					"linecount" : 2,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 14.0, 33.0, 410.0, 34.0 ],
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-7"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "list.delta",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 14.0, 9.0, 153.0, 27.0 ],
					"numoutlets" : 0,
					"fontsize" : 18.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1.1 2.2 3.3 4.4 5.5 7.4",
					"fontname" : "Arial",
					"numinlets" : 2,
					"patching_rect" : [ 29.0, 100.0, 125.330452, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 11.63564,
					"outlettype" : [ "" ],
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj list.delta",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 29.0, 142.0, 75.987755, 20.0 ],
					"numoutlets" : 2,
					"fontsize" : 11.63564,
					"outlettype" : [ "", "" ],
					"id" : "obj-10"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "see other list operators",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 261.0, 167.0, 151.0, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-11"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 274.762817, 129.0, 38.5, 129.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 179.515518, 129.0, 38.5, 129.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
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
