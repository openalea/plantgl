 # -Geom File-
(#--------------------------------------------------
 #  
 #  File : D:/Fred/Mes Documents/Develop/Geom/Leaf/tulipa.geom
 #  
 #  A GEOM file generated with GEOM library
 #  Published under the GNU General Public Licence. 
 #  
 # --------------------------------------------------
 #)

Extrusion GEOM_11276564 { 
    Axis NurbsCurve { 
        CtrlPointList [ 
            <0,0,-1,1>, 
            <0.1,0,3,1>, 
            <0.1,0.1,5,1>, 
            <0,0.1,7,1>
        ]
    }
    CrossSection Polyline2D { 
        PointList [ 
            <0.08,0>, 
            <0.0565685,0.0565685>, 
            <-3.49691e-009,0.08>, 
            <-0.0565685,0.0565685>, 
            <-0.08,-6.99382e-009>, 
            <-0.0565685,-0.0565685>, 
            <9.5399e-010,-0.08>, 
            <0.0565685,-0.0565685>, 
            <0.08,1.39876e-008>
        ]
    }
    Scale [ 
        <1,1>, 
        <1,1>
    ]
}

Material c_green { 
    Ambient <16,34,9>
    Diffuse 2.5
}

Shape SHAPE_11276564 { 
    Geometry  GEOM_11276564
    Appearance  c_green
}

Group GEOM_11195732 { 
    GeometryList [ 
        Translated { 
            Translation <0,0,-0.5>
            Geometry AxisRotated { 
                Axis <0,1,0>
                Angle -136.456
                Geometry Extrusion Leaf { 
                    Axis NurbsCurve { 
                        CtrlPointList [ 
                            <-0.3,0,0.5,1>, 
                            <0,0,0.01,1>, 
                            <0.5,0,0.2,1>, 
                            <1.5,0,0,1>, 
                            <2.5,0,-0.4,1>, 
                            <4,0,-1.7,1>, 
                            <5,0,-1.2,1>
                        ]
                    }
                    CrossSection NurbsCurve2D { 
                        CtrlPointList [ 
                            <-0.2,-1.3>, 
                            <-0.3,-1>, 
                            <0,-1>, 
                            <0.1,-0.3>, 
                            <-0.2,-0.1>, 
                            <-0.4,0>, 
                            <-0.2,0.1>, 
                            <0.1,0.3>, 
                            <0,1>, 
                            <-0.3,1>, 
                            <-0.2,1.3>
                        ]
                    }
                    Scale [ 
                        <0.0333333,0.0333333>, 
                        <0.433333,0.433333>, 
                        <0.583333,0.583333>, 
                        <0.566667,0.566667>, 
                        <0.266667,0.266667>, 
                        <0.0333333,0.0333333>
                    ]
                }
            }
        }, 
        Translated { 
            Translation <0,0,0.1>
            Geometry AxisRotated { 
                Angle 90
                Geometry AxisRotated { 
                    Axis <0,1,0>
                    Angle -138.992
                    Geometry Leaf

                }
            }
        }, 
        Translated { 
            Translation <0,0,-0.5>
            Geometry AxisRotated { 
                Angle 180
                Geometry AxisRotated { 
                    Axis <0,1,0>
                    Angle -120.774
                    Geometry Leaf

                }
            }
        }, 
        Translated { 
            Translation <0,0,-0.1>
            Geometry AxisRotated { 
                Angle 270
                Geometry AxisRotated { 
                    Axis <0,1,0>
                    Angle -138.941
                    Geometry Leaf

                }
            }
        }
    ]
}

ImageTexture APP_11274832 { 
    Filename "leaft.png"
}

Shape SHAPE_11195732 { 
    Geometry  GEOM_11195732
    Appearance  APP_11274832
}

Translated GEOM_11277000 { 
    Translation <0,0.1,7>
    Geometry AxisRotated { 
        Axis <0,1,0>
        Angle -15
        Geometry Scaled { 
            Scale <0.3,0.3,0.1>
            Geometry Sphere { 
            }
        }
    }
}

Material c_red { 
    Ambient <15,5,2>
    Diffuse 3
}

Shape SHAPE_11277000 { 
    Geometry  GEOM_11277000
    Appearance  c_red
}

Translated GEOM_11280032 { 
    Translation <0,0.1,7>
    Geometry AxisRotated { 
        Axis <0,1,0>
        Angle -15
        Geometry Scaled { 
            Scale <0.3,0.3,0.3>
            Geometry Group { 
                GeometryList [ 
                    Translated { 
                        Translation <0,0,-0.2>
                        Geometry AxisRotated { 
                            Axis <0,1,0>
                            Angle -75
                            Geometry BezierPatch petal { 
                                UDegree 2
                                VDegree 4
                                CtrlPointMatrix [
                                     [ <0,0,0,1>, <0,0,0,1>, <0,0,0,1> ],
                                     [ <2,-1,-0.8,1>, <2,0,-1,1>, <2,1,-0.8,1> ],
                                     [ <4,-2,-1.8,1>, <4,0,-2,1>, <4,2,-1.8,1> ],
                                     [ <6,-1.8,-0.5,1>, <6,0,-0.8,1>, <6,1.8,-0.5,1> ],
                                     [ <7,-0.8,-0.1,1>, <8,0,-0.5,1>, <7,0.8,-0.1,1> ]
                                ]
                            }
                        }
                    }, 
                    AxisRotated { 
                        Angle 60
                        Geometry AxisRotated { 
                            Axis <0,1,0>
                            Angle -70
                            Geometry petal

                        }
                    }, 
                    Translated { 
                        Translation <0,0,-0.2>
                        Geometry AxisRotated { 
                            Angle 120
                            Geometry AxisRotated { 
                                Axis <0,1,0>
                                Angle -75
                                Geometry petal

                            }
                        }
                    }, 
                    AxisRotated { 
                        Angle 180
                        Geometry AxisRotated { 
                            Axis <0,1,0>
                            Angle -70
                            Geometry petal

                        }
                    }, 
                    Translated { 
                        Translation <0,0,-0.2>
                        Geometry AxisRotated { 
                            Angle 240
                            Geometry AxisRotated { 
                                Axis <0,1,0>
                                Angle -75
                                Geometry petal

                            }
                        }
                    }, 
                    AxisRotated { 
                        Angle 300
                        Geometry AxisRotated { 
                            Axis <0,1,0>
                            Angle -70
                            Geometry petal

                        }
                    }
                ]
            }
        }
    }
}

ImageTexture APP_11280080 { 
    Filename "petal.png"
}

Shape SHAPE_11280032 { 
    Geometry  GEOM_11280032
    Appearance  APP_11280080
}

