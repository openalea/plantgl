
Text test {
	String "test"
	FontStyle Font {
		Family "Comic Sans MS"
		Size 16
		Bold False
		Italic True
	}
}

Material mat {
	Ambient <230,10,10>
}

Material mat2 {
	Ambient <10,10,230>
}

Shape { test, mat }

Translated deux {
	Translation <0,0,10>
	Geometry Text  {
		String "Deuxième test"
		FontStyle Font {
		}
	}
}

Shape {deux, mat }

Text trois {
		String "Troisieme test"
		Position <5,5,20>
		ScreenCoordinates True
}
Text quatre {
		String "Quatrieme test"
		Position <5,5,80>
		ScreenCoordinates True
}

Shape {trois, mat }
Shape {quatre, mat2 }