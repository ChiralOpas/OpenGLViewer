#version 430 core

// subroutine signature
subroutine vec4 colorRedBlue();

// option for red
subroutine(colorRedBlue) vec4 redColor() {

	return vec4(1.0, 0.0, 0.0, 1.0);
}

// option for blue
subroutine(colorRedBlue) vec4 blueColor() {

	return vec4(0.0, 0.0, 1.0, 1.0);
}

// subroutine variable to use in main
subroutine uniform colorRedBlue theRedBlueSelection;

layout(location = 0) in vec4 position;
uniform mat4 u_mvp;
out vec4 color;

void main()
{
	color = theRedBlueSelection();
	gl_Position = u_mvp * position;
}