edge_xwidth = 3;
edge_zwidth = 3;

xlen = 130 + 2*edge_xwidth;
zlen = 140 + edge_zwidth;
ylen = 35;

cutout_zlen = 20;
cutout_xlen = 65 + edge_xwidth;
cutout_ylen = ylen+1;



inner_cutout_zlen = zlen ;
inner_cutout_xlen = xlen - 2*edge_xwidth;
inner_cutout_ylen = 16;//ylen - 5;

screen_overhang_ylen = 2;

inner_rail_ylen = ylen - inner_cutout_ylen - screen_overhang_ylen - 3;//5;
inner_rail_xlen = 15;
inner_rail_short_xlen = inner_rail_xlen - 7;
inner_rail_short_zlen = 15;

screw_hole_ylen = inner_rail_ylen+5;

board_hole_xlen = 45;
board_hole_ylen = 8;
board_hole_zlen = edge_zwidth + 1;

board_hole_mount_radius = 4;

case_cover_xlen = board_hole_xlen + 6*board_hole_mount_radius + 2*edge_xwidth;
case_cover_ylen = board_hole_ylen + 2*edge_xwidth;
case_cover_inner_ylen = board_hole_ylen + 4;
case_cover_zlen = 24 + edge_zwidth;


//translate([xlen/2, ylen/2, (zlen+edge_zwidth)/2 + zlen - cutout_zlen+0.1]) rotate([0,180,0]) translate([-xlen/2, -ylen/2, -(zlen-edge_zwidth)/2]) case_base();
//case_base();

//case_left();
//translate([xlen/2 - case_cover_xlen/2,screen_overhang_ylen + inner_cutout_ylen - edge_xwidth,-case_cover_zlen]) case_left_cover();

case_left_cover();
//translate([xlen,0,0]) color([1,1,0]) cube([5, 1, 133]);
//translate([xlen,0,133]) color([1,0,1]) cube([5, 1, 133]);

//translate([case_cover_xlen/3,case_cover_ylen/2,edge_zwidth]) cylinder(h=7, r=2.5);



module case_left_cover() {
    union() {
        translate([case_cover_xlen/3,case_cover_ylen/2,edge_zwidth-0.1]) rotate_extrude($fn=20) polygon( points=[[0,0],[2.5,0],[2.5,7],[3.5,9],[3.5,9.5],[0,10]] );
        
    difference() {
        union() {
            cube([case_cover_xlen, case_cover_ylen, case_cover_zlen]);
            translate([edge_xwidth + board_hole_mount_radius, edge_xwidth + board_hole_mount_radius, case_cover_zlen-0.1]) cylinder(h=board_hole_zlen+5, r=board_hole_mount_radius-0.15);
    
            translate([case_cover_xlen - edge_xwidth - board_hole_mount_radius, edge_xwidth + board_hole_mount_radius, case_cover_zlen-0.1]) cylinder(h=board_hole_zlen+5, r=board_hole_mount_radius-0.15);
        };
      
      //translate([edge_xwidth, edge_xwidth, edge_zwidth+0.1]) cube([case_cover_xlen - 2* edge_xwidth, case_cover_ylen - 2 * edge_xwidth, case_cover_zlen - edge_zwidth]);
        
        translate([edge_xwidth + 3 * board_hole_mount_radius,(case_cover_ylen - case_cover_inner_ylen)/2,edge_zwidth + 0.1]) cube([board_hole_xlen, case_cover_inner_ylen, case_cover_zlen - edge_zwidth]);
        
        angle_xlen = 30;
        angle_zlen = 30;
        rotate([0, 45, 0]) translate([-angle_xlen/2,-0.5,-angle_zlen/2]) cube([angle_xlen,case_cover_ylen+1,angle_zlen]);
        
        translate([case_cover_xlen,0,0]) rotate([0, 45, 0]) translate([-angle_xlen/2,-0.5,-angle_zlen/2]) cube([angle_xlen,case_cover_ylen+1,angle_zlen]);
        
        tri_x = 8.45;//5.65;
        tri_z = 8.45;//5.65;
        translate([tri_x*1.5,edge_xwidth+4,edge_zwidth+3.2]) rotate([0,45,90]) translate([-tri_x/2,0,-tri_z/2]) polyhedron ( points = [[0, -5, tri_z], [0, 5, tri_z], [0, 5, 0], [0, -5, 0], [tri_x, -5, tri_z], [tri_x, 5, tri_z]], 
    triangles = [[0,3,2], [0,2,1], [3,0,4], [1,2,5], [0,5,4], [0,1,5],  [5,2,4], [4,2,3], ]);
        
        translate([case_cover_xlen - tri_x*1.5,edge_xwidth+4,edge_zwidth+3.2]) rotate([0,45,90]) translate([-tri_x/2,0,-tri_z/2]) polyhedron ( points = [[0, -5, tri_z], [0, 5, tri_z], [0, 5, 0], [0, -5, 0], [tri_x, -5, tri_z], [tri_x, 5, tri_z]], 
    triangles = [[0,3,2], [0,2,1], [3,0,4], [1,2,5], [0,5,4], [0,1,5],  [5,2,4], [4,2,3], ]);
        
    };        
};
};

module case_left() {
    
    
    difference() {
        case_base();
        
        translate([xlen/2 - board_hole_xlen/2,screen_overhang_ylen + inner_cutout_ylen,-0.5]) cube([board_hole_xlen, board_hole_ylen, board_hole_zlen]);
        
        translate([xlen/2 - board_hole_xlen/2 - 2*board_hole_mount_radius, screen_overhang_ylen + inner_cutout_ylen + board_hole_mount_radius, -0.5]) cylinder(h=board_hole_zlen, r=board_hole_mount_radius);
    
        translate([xlen/2 + board_hole_xlen/2 + 2*board_hole_mount_radius, screen_overhang_ylen + inner_cutout_ylen + board_hole_mount_radius, -0.5]) cylinder(h=board_hole_zlen, r=board_hole_mount_radius);
    };
    
    
};

module case_base() {
    difference() {
        cube([xlen, ylen, zlen]);
        
        translate([-0.5,-0.5,zlen-cutout_zlen-0.1]) color([1,0,0]) cube([cutout_xlen + 0.6,cutout_ylen,cutout_zlen + 0.5]);
        
        translate([edge_xwidth, screen_overhang_ylen, edge_zwidth]) color([0, 1, 0]) cube([inner_cutout_xlen, inner_cutout_ylen, inner_cutout_zlen]);
        
        //translate([edge_xwidth, screen_overhang_ylen + inner_cutout_ylen + inner_rail_ylen, edge_zwidth]) color([0, 1, 0]) cube([inner_cutout_xlen, ylen - inner_cutout_ylen - inner_rail_ylen - 5, inner_cutout_zlen]);
        
        translate([edge_xwidth + inner_rail_short_xlen, screen_overhang_ylen + inner_cutout_ylen - 0.5, edge_zwidth+inner_rail_short_zlen]) color([0, 1, 0]) cube([inner_cutout_xlen - inner_rail_xlen - inner_rail_short_xlen, inner_rail_ylen + 0.5, inner_cutout_zlen]);
        
        translate([edge_xwidth + inner_rail_xlen, screen_overhang_ylen + inner_cutout_ylen - 0.5, edge_zwidth]) color([0, 1, 0]) cube([inner_cutout_xlen - 2*inner_rail_xlen, inner_rail_ylen + 0.5, inner_rail_short_zlen+0.5]);
        
        translate([edge_xwidth+2, screen_overhang_ylen-4, edge_zwidth+2]) color([0, 0, 1]) cube([inner_cutout_xlen - 4, inner_cutout_ylen, inner_cutout_zlen]);
        
        translate([xlen - edge_xwidth - 10,inner_cutout_ylen + screen_overhang_ylen+screw_hole_ylen/2,edge_zwidth+ 10 + 120]) screw_hole();

        translate([xlen - edge_xwidth - 10,inner_cutout_ylen + screen_overhang_ylen+screw_hole_ylen/2,edge_zwidth+ 10]) screw_hole();

        translate([edge_xwidth + 8,inner_cutout_ylen + screen_overhang_ylen+screw_hole_ylen/2,edge_zwidth+ 10]) screw_hole();
        
        translate([xlen/4,ylen,zlen/7]) mount_hole();
        translate([xlen*3/4,ylen,zlen/7]) mount_hole();
    };
};

module screw_hole() {
    //translate([xlen - edge_xwidth - 6,inner_cutout_ylen + screen_overhang_ylen+screw_hole_ylen/2-1,zlen - 10]) rotate([90,0,0]) translate([0,0,-screw_hole_ylen/2]) color([0,0,1]) cylinder(h=screw_hole_ylen, r=1.5);

    //translate([xlen - edge_xwidth - 6,inner_cutout_ylen + screen_overhang_ylen+screw_hole_ylen/2+3,zlen - 10]) rotate([90,0,0]) translate([0,0,-screw_hole_ylen/2]) color([0,0,1]) cylinder(h=screw_hole_ylen, r=2.5);
    
    
    translate([0,-1,0]) rotate([90,90,0]) translate([0,0,-screw_hole_ylen/2]) color([0,0,1]) cylinder(h=screw_hole_ylen, r=2.0);

    translate([0,3,0]) rotate([90,90,0]) translate([0,0,-screw_hole_ylen/2]) color([0,0,1]) cylinder(h=screw_hole_ylen, r=3);
};

module mount_hole() {
    tri_x = 7;
    tri_z = 7;
    union() {
        translate([-0.1,0,0]) rotate([0,135,0]) translate([-tri_x/2,0,-tri_z/2]) polyhedron ( points = [[0, -5, tri_z], [0, 5, tri_z], [0, 5, 0], [0, -5, 0], [tri_x, -5, tri_z], [tri_x, 5, tri_z]], 
    triangles = [[0,3,2], [0,2,1], [3,0,4], [1,2,5], [0,5,4], [0,1,5],  [5,2,4], [4,2,3], ]);
        rotate([0,-45,0]) translate([-tri_x/2,0,-tri_z/2]) polyhedron ( points = [[0, -5, tri_z], [0, 5, tri_z], [0, 5, 0], [0, -5, 0], [tri_x, -5, tri_z], [tri_x, 5, tri_z]], 
    triangles = [[0,3,2], [0,2,1], [3,0,4], [1,2,5], [0,5,4], [0,1,5],  [5,2,4], [4,2,3], ]);
    };
};