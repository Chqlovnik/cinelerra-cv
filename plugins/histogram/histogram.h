#ifndef HISTOGRAM_H
#define HISTOGRAM_H


#include "histogram.inc"
#include "histogramconfig.h"
#include "histogramwindow.inc"
#include "loadbalance.h"
#include "plugincolors.h"
#include "pluginvclient.h"


class HistogramMain : public PluginVClient
{
public:
	HistogramMain(PluginServer *server);
	~HistogramMain();

	int process_realtime(VFrame *input_ptr, VFrame *output_ptr);
	int is_realtime();
	int load_defaults();
	int save_defaults();
	void save_data(KeyFrame *keyframe);
	void read_data(KeyFrame *keyframe);
	void update_gui();
	void render_gui(void *data);

	PLUGIN_CLASS_MEMBERS(HistogramConfig, HistogramThread)

// Convert input to linear output
	float calculate_linear(float input, int mode, int do_value);
	float calculate_smooth(float input, int subscript);
// Convert input to smoothed output by looking up in smooth table.
	float calculate_curve(float input);
// Calculate automatic settings
	void calculate_automatic(VFrame *data);
// Calculate histogram
	void calculate_histogram(VFrame *data);
// Calculate the linear, smoothed, lookup curves
	void tabulate_curve(int subscript, int use_value);



	YUV yuv;
	VFrame *input, *output;
	HistogramEngine *engine;
	int *lookup[HISTOGRAM_MODES];
	float *smoothed[HISTOGRAM_MODES];
	float *linear[HISTOGRAM_MODES];
	int *accum[HISTOGRAM_MODES];
// Input point being dragged or edited
	int current_point;
// Current channel being viewed
	int mode;
	int dragging_point;
	int point_x_offset;
	int point_y_offset;
};

class HistogramPackage : public LoadPackage
{
public:
	HistogramPackage();
	int start, end;
};

class HistogramUnit : public LoadClient
{
public:
	HistogramUnit(HistogramEngine *server, HistogramMain *plugin);
	~HistogramUnit();
	void process_package(LoadPackage *package);
	HistogramEngine *server;
	HistogramMain *plugin;
	int *accum[5];
};

class HistogramEngine : public LoadServer
{
public:
	HistogramEngine(HistogramMain *plugin, 
		int total_clients, 
		int total_packages);
	void process_packages(int operation, VFrame *data);
	void init_packages();
	LoadClient* new_client();
	LoadPackage* new_package();
	HistogramMain *plugin;
	int total_size;


	int operation;
	enum
	{
		HISTOGRAM,
		APPLY
	};
	VFrame *data;
};











#endif