/* -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
 * vi:set ts=4 sts=4 sw=4 noet :
 *
 * Copyright 2010, 2011 wkhtmltopdf authors
 *
 * This file is part of wkhtmltopdf.
 *
 * wkhtmltopdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * wkhtmltopdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with wkhtmltopdf.  If not, see <http: *www.gnu.org/licenses/>.
 */

/* This is a test version using local HTML files instead of remote URLs */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wkhtmltox/image.h>

/* Print out loading progress information */
void progress_changed(wkhtmltoimage_converter * c, int p) {
	printf("%3d%%\r",p);
	fflush(stdout);
}

/* Print loading phase information */
void phase_changed(wkhtmltoimage_converter * c) {
	int phase = wkhtmltoimage_current_phase(c);
	printf("%s\n", wkhtmltoimage_phase_description(c, phase));
}

/* Print a message to stderr when an error occurs */
void error(wkhtmltoimage_converter * c, const char * msg) {
	fprintf(stderr, "Error: %s\n", msg);
}

/* Print a message to stderr when a warning is issued */
void warning(wkhtmltoimage_converter * c, const char * msg) {
	fprintf(stderr, "Warning: %s\n", msg);
}

/* Main method convert image */
int main() {
	wkhtmltoimage_global_settings * gs;
	wkhtmltoimage_converter * c;
	const unsigned char * data;
	long len;
	int ret;

	/* Init wkhtmltoimage in graphics less mode */
	wkhtmltoimage_init(false);

	/*
	 * Create a global settings object used to store options that are not
	 * related to input objects, note that control of this object is parsed to
	 * the converter later, which is then responsible for freeing it
	 */
	gs = wkhtmltoimage_create_global_settings();

	/* We want to convert the local test HTML file */
	wkhtmltoimage_set_global_setting(gs, "in", "test_image.html");
	wkhtmltoimage_set_global_setting(gs, "out", "test_output.jpg");
	wkhtmltoimage_set_global_setting(gs, "fmt", "jpeg");

	/* Create the actual converter object used to convert the pages */
	c = wkhtmltoimage_create_converter(gs, NULL);

	/* Call the progress_changed function when progress changes */
	wkhtmltoimage_set_progress_changed_callback(c, progress_changed);

	/* Call the phase _changed function when the phase changes */
	wkhtmltoimage_set_phase_changed_callback(c, phase_changed);

	/* Call the error function when an error occurs */
	wkhtmltoimage_set_error_callback(c, error);

	/* Call the warning function when a warning is issued */
	wkhtmltoimage_set_warning_callback(c, warning);

	/* Perform the actual conversion */
	ret = wkhtmltoimage_convert(c);
	if (!ret) {
		fprintf(stderr, "Conversion failed!\n");
	} else {
		printf("Conversion succeeded!\n");
	}

	/* Output possible http error code encountered */
	printf("httpErrorCode: %d\n", wkhtmltoimage_http_error_code(c));

	len = wkhtmltoimage_get_output(c, &data);
	printf("Output length: %ld bytes\n", len);

	/* Destroy the converter object since we are done with it */
	wkhtmltoimage_destroy_converter(c);

	/* We will no longer be needing wkhtmltoimage funcionality */
	wkhtmltoimage_deinit();

	return ret ? 0 : 1;
}
