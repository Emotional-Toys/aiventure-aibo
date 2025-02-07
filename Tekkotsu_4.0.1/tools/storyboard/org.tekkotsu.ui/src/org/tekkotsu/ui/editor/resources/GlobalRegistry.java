package org.tekkotsu.ui.editor.resources;

public class GlobalRegistry {
    static public class ParamObject {
		String dogname = "";
		public void setDogName(String name) {
			dogname = name;
		}
		
		public String getDogName() {
			return dogname;
		}
    }
}
