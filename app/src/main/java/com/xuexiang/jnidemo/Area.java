package com.xuexiang.jnidemo;

/**
 * @author xuexiang
 * @since 2018/10/16 下午4:24
 */
public class Area {

    private int width;
    private int height;

    public Area(int width, int height) {
        this.width = width;
        this.height = height;
    }

    public int getArea() {
        return width * height;
    }
}
