#define WIN32_LEAN_AND_MEAN

#include "glt_stream_server_t.hpp"
#include "glt_stream_source_t.hpp"


glt_stream_server_t::glt_stream_server_t(int w, int h) : GlutWindow("opengl server", w, h, 100, 100, GLUT_RGBA | GLUT_DEPTH){
	width = w;
	height = h;

	xcam = 0;
	ycam = 0;
	zcam = -3;

	trackball(quat, 0, 0, 0, 0);

	m_frame_buffer.reset(new all::core::uint8_t[width * height * 4]);
	image.reset(new all::core::uint8_t[width * height * 3]);

	m_stream_source.reset(new glt_stream_source_t(this));
	m_stream_server.reset(new all::core::stream_server_t(m_stream_source, "config/gl_stream_server.ini"));

	m_stream_server->run_async();

}


void glt_stream_server_t::OnDisplay() {

	build_trans_matrix();
    
    if (draw_data)
		draw_data();

	glFlush();

	update_buffer();

}

void glt_stream_server_t::OnOpen() {
	
	init_gl();

	reset_projection_mode();

	setSwapBuffers(false);

	glDrawBuffer(GL_FRONT);

	setTick(100);

}

void glt_stream_server_t::OnClose() {
}

void glt_stream_server_t::init_gl() {

	static const GLfloat light0_pos[4]   = { -50.0f, 50.0f, 0.0f, 0.0f };

    // white light
    static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
    /* light */
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);

    glEnable(GL_LIGHT0);
    /* remove back faces */
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* speedups */
    //glEnable(GL_DITHER);
    //glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

  	// Point Size
    float pointSize = 2.2;
    glPointSize(pointSize);

}

void glt_stream_server_t::reset_projection_mode() {
	glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0f, (GLfloat)width/height, 0.2, +100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glt_stream_server_t::build_trans_matrix() {
	glLoadIdentity();
    
	glTranslatef( xcam, ycam, zcam);

    GLfloat m[4][4];
    build_rotmatrix( m, quat );
    glMultMatrixf( &m[0][0] );
}

void glt_stream_server_t::update_buffer() {

	boost::mutex::scoped_lock lock (mutex);

	glReadBuffer(GL_FRONT);
	glReadPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,(void*) m_frame_buffer.get() );

	all::core::change_ordering::from_rgba_opengl_to_planar(m_frame_buffer, image, height, width);
    all::core::change_ordering::to_topleft(image, height, width, 3);

}