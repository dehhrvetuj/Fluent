/*
 * Copyright ANSYS. All Rights Reserved.
 */

#ifndef _FLUENT_DPM_H
#define _FLUENT_DPM_H

#include "global.h"
#include "mem.h"
#include "profile.h"
#include "slide.h"
#include "cx.h"

/**
 * @addtogroup dpm Discrete Particle Model
 * @{
 *
 * @file
 * Interface to Discrete Particle Model (independent on internal types)
 */

#define DPM_SMALL 1.0e-9
#if RP_3D
# define DPM_VELOCITY_DIM ND_ND
#else
# define DPM_VELOCITY_DIM (rp_axi_swirl ? 3 : ND_ND)
#endif

#define MAX_REACT_POST 20 /* always set less than MAX_SPE_EQNS */
#define MAX_DPM_MATERIAL 100

/* boundary conditions */
/* DEM ubecker  Replace explicit macro definitions by enum defined below.
 */
/* #define DPM_BC_INTERIOR    1 */
/* #define DPM_BC_REFLECT     2 */
/* #define DPM_BC_TRAP        3 */
/* #define DPM_BC_ESCAPE      4 */
/* #define DPM_BC_SALTATE     5 */
/* #define DPM_BC_PERIODIC    6 */
/* #define DPM_BC_ABORT       7 */
/* #define DPM_BC_SYMMETRY    8 */
/* #define DPM_BC_SLIDING     9 */
/* #define DPM_BC_WALL_JET   10 */
/* #define DPM_BC_UDF        11 */
/* #define DPM_BC_PARALLEL   12 */
/* #define DPM_BC_PARALLEL_PERIODIC  13 */
/* #define DPM_BC_WALL_FILM  14 */
/* #if RP_WALL_FILM && RP_3D */
/* #  define DPM_BC_WALL_EFILM 15 */
/* #endif */

/** DEM Have an enum for types of DPM BCs.
 *
 * ATTENTION: Any change here incures changing getintersectingface_secant accordingly.
 */
enum dpm_bc_types
  {
    DPM_BC_INTERIOR        = 1,
    DPM_BC_REFLECT    	      , /*  2 */
    DPM_BC_TRAP       	      , /*  3 */
    DPM_BC_ESCAPE     	      , /*  4 */
    DPM_BC_SALTATE    	      , /*  5 */
    DPM_BC_PERIODIC   	      , /*  6 */
    DPM_BC_ABORT      	      , /*  7 */
    DPM_BC_SYMMETRY   	      , /*  8 */
    DPM_BC_SLIDING    	      , /*  9 */
    DPM_BC_WALL_JET   	      , /* 10 */
    DPM_BC_UDF        	      , /* 11 */
    DPM_BC_PARALLEL   	      , /* 12 */
    DPM_BC_PARALLEL_PERIODIC  , /* 13 */
    DPM_BC_WALL_FILM          , /* 14 */
#if RP_WALL_FILM && RP_3D
    DPM_BC_WALL_EFILM         , /* 15 */
#endif
    DPM_BC_TYPES_MAX         /* one more than true number of types */
  };

typedef enum
{
  FILM_NONE = -1,
  FILM_STICK,
  FILM_REBOUND,
  FILM_SPREAD,
  FILM_SPLASH,
  FILM_USER_0,
  FILM_USER_1,
  FILM_USER_2,
  FILM_USER_3,
  FILM_USER_4,
  FILM_USER_5,
  FILM_USER_6,
  FILM_USER_7,
  FILM_USER_8,
  FILM_USER_9,
  MAX_FILM_REGIME
} Wall_Film_Regime;

typedef enum
{
  IMPINGEMENT_DEFAULT = 0,
  IMPINGEMENT_KUHNKE,
  IMPINGEMENT_DEFAULT_EWF,
  IMPINGEMENT_STANTON_UNTESTED
} Wall_Impingement_Type;

#define MIN_LOG_EA -10 /* from Kuhnke plots 4.11 and 4.6 */

typedef enum
{
  EROSION_GENERIC = 0,
  EROSION_FINNIE,
  EROSION_MCLAURY,
  EROSION_OKA,
  DPM_MAX_EROSION
} Wall_Erosion_Models;

typedef enum
{
  FOUR_PAR_LH = 0,
  THREE_PAR_LH,
  ROSIN_RAMMER,
  LOGISTIC,
  WEIBULL,
  NUKIYAMA_TANASAWA,
  MAX_SPLASH_DISTRIBUTIONS
} Splashing_Distribution_Type;

#define SPLASH_VEL(S,I)((S)->vel[I])
#define SPLASH_N(S,I)((S)->npar[I])
#define SPLASH_DIAM(S,I)((S)->diam[I])

/* forward declaration <dpm_types.h>
   Some models use particle pointers without needing to know how the struct is defined. */
struct particle_struct;

/**
 * @addtogroup dpm_interface interface functions/variables
 * @{ */

#define C_DPM_VIS0(c,t) C_STORAGE_R(c,t,SV_DPM_VIS_0)

/**
 * @addtogroup dpm_interface_global accessible global variables
 * @{ *************************************************************/

typedef enum
{
  EULER_IMPLICIT = 0,
  EULER_TRAPEZOIDAL,
  EXPONENTIAL_ANALYTIC,
  RK_EXPLICIT,
  EULER_SYMPLECTIC,
  EXPONENTIAL_ANALYTIC_DIAG,
  MAX_TRACKING_SCHEME
} Tracking_Scheme_Type;


typedef enum
  {
    DPM_COLLISION_MESH_BARE_CFD=0,
    DPM_COLLISION_MESH_CARTESIAN,
    DPM_COLLISION_MESH_UNDEFINED
  } DPM_Collision_Mesh_Type;


#define INJECTION_SPRAY_SUITE_DDRAG(I) (DPM_DRAG_SPRAY_SUITE == (I)->drag_law)

#define NUM_SPRAY_CONSTS 13
typedef struct spray_suite_struct
{
  cxboolean break_up;
  cxboolean tab;
  cxboolean wave;
  cxboolean ssd;
  cxboolean collide;
  cxboolean coalescence;
  cxboolean ddrag;
  cxboolean khrt;       /* Kelvin-Helmholtz--Rayleigh-Taylor model (ASME 97-ICE-52) */
  cxboolean wave_collide;  
  cxboolean wave_mass_cutoff_adjust;  
  cxboolean wave_rayleigh_growth;  
  cxboolean tab_evaluate_double_ddrag; 
  cxboolean collision_hybridparallelP;
  cxboolean collision_unidirectionalP;
  cxboolean collision_hybpar_everyield;

  int collision_mode;
  int collision_model_type;
  int collision_event_type;
  DPM_Collision_Mesh_Type collision_mesh_type;
  int collision_stencil_size;
  int n_coeff_restitution;

  real coeff_restitution[2];
  real constant[NUM_SPRAY_CONSTS];
  /* CK CD CB CF KENRG CV CONSTANT_1 B_1 B_2 Init_TAB_Amp */
  /* KHRT-c-levich KHRT-c-tau KHRT-c-3 */
  /* Note that CONSTANT_1 has been removed from the gui/tui - not for users */
  real wave_mass_cutoff;
  real min_time_step;  /* lowest time step for NANBU collision model */
  /* ssd spray model parameters */
  /* real ssd_we_cr;     -- now injection-specific */
  real ssd_min_diam;
  /* real ssd_np_target; -- now injection-specific */
  /* real ssd_core_bu;   -- now injection-specific */
  real ssd_x_ratio;
  /* real ssd_x_si;      -- now injection-specific */
  real ssd_nu_t;  
} spray_suite_t;

typedef struct impingement_constant_struct
{
  real critical_transition_factor;
  real E_2_crit[MAX_FILM_REGIME]; /* critical constants for impingement regimes definition */
  real KTmin;
  real KTmax;
  real cT;
  real We_limit;
} impingement_constant_t;

typedef struct splashing_distribution_struct
{
  real **vel;
  real *npar;
  real *diam;

} splashing_distribution_t;

typedef struct dpm_parameters_struct
{
  cxboolean radiation_p;
  cxboolean use_len_scale;
  real len_scale;
  real len_factor;
  int max_steps;
  int correlation_func; /* correlation function for DPM cloud model*/
  cxboolean ensemble_average; /* do ensemble-average for DPM cloud model*/
  int n_user_reals;
  cxboolean unsteady_tracking;
  cxboolean reallocate_unsteady_coupled; 
  real time;
  real particle_time_step;
  real time_step;
  real last_time_step;
  int n_time_steps;
  cxboolean create_with_particle_time_step;
  cxboolean fluid_flow_time_stepping;
  cxboolean thermophoretic_force;
  cxboolean pressure_force_explicit;
  cxboolean pressure_force;
  cxboolean virtual_mass;
  real virtual_mass_factor;
  int drag_law;   /* will be set if uniform across all injections. */
  /* real shape_factor;   -- now injection-specific
   * cxboolean brownian_motion;
   * real cunningham_correction;
   */
  cxboolean saffman_lift;
  cxboolean binary;
  FILE *fp;
  cxboolean track_in_absolute;
#if 0
  cxboolean coupled_update; /* not used anymore */
#endif
  cxboolean coupled_update_droplet;  
  cxboolean coupled_update_combusting;
  real Tmin, Tmax;
  Tracking_Scheme_Type tracking_scheme;  /* actual tracking scheme */
  Tracking_Scheme_Type ts_low;       /* tracking_scheme for large steps and low accuracy */
  Tracking_Scheme_Type ts_high;      /* tracking scheme for small steps and high accuracy */
  Tracking_Scheme_Type film_tracking_scheme;         /* tracking scheme for film particles */
  cxboolean step_size_control;
  cxboolean path_line_step_control;
  cxboolean second_order_pathlines;
  Tracking_Scheme_Type pathline_scheme;
  cxboolean track_spawned_particles;  /* consider spawned particles during the same tracking step */
  int display_steps;
  int max_step_size_refinements;
  cxboolean tracking_scheme_selection;
  cxboolean limit_min_time_step;
  cxboolean injections_have_positive_flowrate;
  real max_error;
  real multicomponent_tolerance;
  int multicomponent_max_nsteps;
  int multicomponent_max_nred;
  real cells_to_cross;  /* maximum number of cells to be crossed per step */
  real lowest_diam;
  real lowest_film_diam;
  real leidenfrost_dt;
  real lowest_mass;
  real minimum_vapor_fraction;
  real mut_to_mul;
  real eddy_cross_time_factor;
  cxboolean rkvaporization;
  cxboolean rkmulticomponent;
  real fractional_change_factor_mass;
  real fractional_change_factor_heat;
  cxboolean limiting_time_algorithm;
  int surface_composition_assumption;
  cxboolean absolute_pressure;
  cxboolean latent_heat_at_boiling;
  cxboolean boiling_pressure_depend;
  cxboolean latent_heat_temperature_depend;
  cxboolean allow_condensation;
  cxboolean allow_supercritical_pressure;
  cxboolean high_temperature_correction;
  int face_intersection; /* algorithm choice */
  int pathline_face_intersection;
  int locate_method; /* as used in dpm+path.c:NextPos */
  cxboolean acceleration_to_faces_for_rk; /* consider acceleration terms during
					   computation of face intersection for
					   RK_EXPLICIT and EXPONENTIAL_ANALYTIC */
  int particle_repartition;   /* algorithm choice */
  int debug_output;
  cxboolean temperature_limiter;

  cxboolean search_before_iteration;
  cxboolean closest_cell_search;
  cxboolean closest_face_search;
  cxboolean disallow_aborted_particles;
  cxboolean use_face_v_with_b_injection;
  cxboolean pre_write_particle_check;
  cxboolean pre_write_particle_project;
  cxboolean pre_write_particle_project_dem;
  cxboolean relocate_init_particles;

  cxboolean wall_jet_interact;
  cxboolean wall_film_active;
  cxboolean wall_film_expert;
  cxboolean film_node_values;
  real film_separation_angle;
  real film_separation_height;
  cxboolean film_visc_interaction;
  cxboolean film_gas_interaction;
  real film_mean_interaction;
  real film_p_imp_interaction;
  real film_m_imp_interaction;
  real film_min_dsplash_param;
  real film_splash_angle;
  real film_splash_angle_slope;
  real max_wallfilm_dist;
  cxboolean wall_film_heat_transfer;
  real limiting_film_height;
  cxboolean film_boiling_rate;
  cxboolean film_formulation_r15;
  cxboolean use_previous_film_height;
  cxboolean film_interpolate_mesh;
  impingement_constant_t impingement_constant;
  real contact_time_factor;
  real ft_limiter;
  real orourke_splash_fraction_tail;
  cxboolean orourke_splash_fraction;
  cxboolean splash_pdf_tail_limiting;
  real splash_size_limit_low;
  real splash_size_limit_high;
  real splash_vel_limit_n;
  real splash_vel_limit_a;
  cxboolean limit_splash_film_height;
  cxboolean limit_kuhnke_init_film_height;
  cxboolean static_film_condensation;
  cxboolean stagger_temporally;
  real stagger_factor;
  real stagger_radius_std_inj;
  int  stagger_spatial_n_tries;
  cxboolean solid_cone_uniform_mass_distribution;
  cxboolean atomizer_uniform_mass_distribution;
  int atomizer_downstream_iterations;

  spray_suite_t spray_suite;

  cxboolean levich_breakup_model;
  int levich_breakup_type;
  real levich_breakup_const;
  cxboolean all_p_in_liquid_core;
  
  int n_scalars;
  int n_cbk; /*CBK model integration constants*/
  cxboolean implicit_momentum_coupling;
  cxboolean old_implicit_momentum_coupling;
  cxboolean sources_every_flow_iteration;
  cxboolean linearized_source_terms;
  cxboolean variable_interpolation;
  cxboolean temperature_interpolation;
  cxboolean interpolate_in_time;
  cxboolean interpolate_nodes;
  int       interpolate_kernel;
  int       interpolate_stencil;
  real      interpolate_gauss;
  cxboolean interpolate_p;
  cxboolean interpolate_vel;
  cxboolean interpolate_temp;
  cxboolean interpolate_turb;
  cxboolean interpolate_granular;
  cxboolean interpolate_memory_initialized;
  int       interp_n_vars;
  int       svar_iv_index[SV_MAX];
  Svar      *interp_vars;
  Svar      *interp_vars_node;
  int       *interp_vars_node_dim;
  int       interp_n_reals;
  
  cxboolean velocity_wall_interpolation;
  cxboolean sources_implicit;
  /** alternate source term formulation used by dense flow model*/
  cxboolean eulerian_source_term_coupling;
  cxboolean consider_discrete_phase_vof;
  cxboolean relocate_unsteady_particles_for_mdm; /* DE21583 */
  /** averaging of cell based variables */
  cxboolean avg_nodes;
  int avg_kernel;
  cxboolean avg_sources; /* node based averages of source terms */
  cxboolean avg_ddpm_vars; /* node based averages of DDPM variables */
  cxboolean avg_each_step; /* do averaging after each integration step */
  real avg_gauss;
  cxboolean short_wave_airblast;
  real atomizer_spread_param;

  /* pdf transport */
  cxboolean number_control;
  cxboolean position_correction;
  cxboolean local_time_step;
  int mixing_model;
  int num_pdf_steps;
  int num_fv_steps;
  int i_ta;
  int micro_mixing;
  real n_ta, inc_ta;
  real num_part_per_cell;
  real ppc_consts[4];
  real C_phi;
  real pdft_cfl;
  real pdft_fourier;
  real pdft_freq;
  real pdft_time_step;

  cxboolean par_dpm, par_mig;
  cxboolean homogeneous_net;
  cxboolean cell_face_weighting;
  cxboolean injections_defined;
  cxboolean particles_in_domain;
  cxboolean source_non_zero;
  
  cxboolean random_tab_diam;
  /* int number_tab_diameters;   -- now injection-specific */
  real lower_tab_diam_ratio;
  real tab_spread_param;
  int tracking_order_randomizations;
  cxboolean random_uniform;
  real uniform_random;
  cxboolean random_gauss;
  real gauss_random;
  cxboolean cp_correction;
  cxboolean multicomponent_boiling;
  cxboolean density_multispecies;
  cxboolean dpm_vof;
  cxboolean fill_sources_per_material;
  int dpm_vof_scheme_type;
  cxboolean dpm_vof_velocity_correction;
  real dpm_vof_gran_temp_lower_limit;
  int n_dpm_domains;
  cxboolean dpm_vof_show_internal_variables;
  cxboolean dpm_vof_euler_check_neighbour_vof;
  cxboolean dpm_vof_euler_allow_parallel_hybrid;
  cxboolean reset_partition_weights;
  cxboolean display_free_particles;
  cxboolean display_wallfilm_particles;
  int verbosity;
  cxboolean is_pathline;  /* we are doing the pathline plot */
  int polyhedron_tet_division_level; /* 0 = no division, 1 = based on faces, 2 = based on nodes */
  cxboolean export_history; /* keep particle id and eventually flow time in history file */
  cxboolean export_periodic_points;
  int n_time; /* number of the current flow time step */
  int n_dpm_time; /* number of the current dpm time step (rpvar 'dpm/time-step) */
  cxboolean sub_cycling_no_update; /* indicates status during sub cycling within a time step */
  cxboolean parallel_hybrid; /* switch for parallel hybrid model */
  cxboolean dem_hybrid; /* switch for dem hybrid model */
  int dem_part_method; /* switch for dem part method, 0-1d, 1-metis */
  real parallel_imbalance_tolerant; /* the allowable group size deviation */
  int parallel_group_size_minimum; /* the minimum allowable group size */
  int parallel_nthreads; /* the number of threads */
  int parallel_lb_method; /* the type of load balance method */
  void ***omp_mem; /* the thread specific memory */
  int nomp_mem; /* the number of thread specific memory */
  size_t lock_count;
  int cfunc_level;      /* us20658 */
  int cfunc_level_last; /* us20658 */
  cxboolean unsteady_statistics;
  cxboolean do_dpm_per_inj_thread_summaries;
  cxboolean use_particle_random_seeds;
  cxboolean serial_particle_id;
  cxboolean use_iteration_seed;
  cxboolean use_timestep_seed;
#if ! UDF_COMPILER /* disable for interpreted udfs */
  int64_t next_part_id;
#endif
  cxboolean breakup_new_partid;
  cxboolean do_rotation; /* used to control visibility of postprocessing variables */
  cxboolean visualize_rotation; /* will require additional memory for storing quaternion (4-tuple array) per particle. */

  /* DEM */
  real dem_max_van_der_waals_range;
  char pl_velocity_domain[MATERIAL_NAME_LENGTH]; /* velocity domain for pathlines. Use name instead of
						    domain index since "all-phases" is also an option. */

  /* Erosion - MDM coupling and boundary face smoothing */
  cxboolean couple_erosion_mdm;
  int face_smoothing_number_of_sweeps;
  int face_smoothing_weighting_option;
  cxboolean face_smoothing_conservative;
  real wall_material_density;
  real erosion_mdm_termination_total_time;
  real erosion_mdm_termination_max_disp;
  real erosion_mdm_mesh_max_disp;
  int erosion_mdm_ref_erosion_model;
  int erosion_mdm_simulation_cntl_num_flow_iterations;
} dpm_parameters_t;

enum
{
  OM_OBJP_MANAGER = 0,
  OM_UNIFORM_RANDOM,
  OM_MEMORY_MANAGER,
  OM_MAX
};


#define SG_NODE_INTERPOLATE_IN_TIME(domain) (sg_mesh_interpolate_in_time && SV_ALLOCATED_P_NEW ((Get_Node_Thread (domain)), SV_N_X_M1))

/* informs about first DPM iteration per time step, always true for steady flows */
FLUENT_EXPORT cxboolean DPM_First_Iteration_P(void);
extern FLUENT_EXPORT dpm_parameters_t dpm_par;
extern FLUENT_EXPORT int DPM_Phase_Id;

/* @} dpm_interface_global */

/**
 * @addtogroup dpm_interface_storage storage definition section
 * @{ *************************************************************/
FLUENT_EXPORT void init_dpm_scalar_on_node(Var_Attribute *sv, Domain *);
FLUENT_EXPORT void init_dpm_vector_on_node(Var_Attribute *sv, Domain *);
FLUENT_EXPORT void init_dpms_species_on_node(Var_Attribute *sv, Domain *);
/* @} dpm_interface_storage */

/**
 * @addtogroup dpm_interface_solver routines called from flow solver or cortex(cxhost/cxnode)
 * @{ *************************************************************/

FLUENT_EXPORT void SV_FlushDPMSources (void);
FLUENT_EXPORT void DPM_Set_Particles_In_Domain (void);
FLUENT_EXPORT void Shift_Unsteady_Particles_In_Cell_Thread(Thread *, STrans * const);
FLUENT_EXPORT void DPM_Reset_Iter(void);
FLUENT_EXPORT void Unsteady_PDF_Transport(Domain *domain);
FLUENT_EXPORT void Move_Unsteady_Particles(real dtime);
FLUENT_EXPORT void InitializeDPMPrimitives(void);
FLUENT_EXPORT void DPM_Update(Domain *);
FLUENT_EXPORT void Model_Initialize_dpm(void);
FLUENT_EXPORT void Init_Unsteady_Particles(void);
FLUENT_EXPORT void Correct_PDF_Transport_Density(Domain *domain);
FLUENT_EXPORT void Create_Unsteady_Particles(real , cxboolean );
FLUENT_EXPORT cxboolean need_dpm_surften_mu(void);
FLUENT_EXPORT void DPM_Convert_Transient_Swirl_Component(void);
FLUENT_EXPORT void DPM_Momentum_Source(Domain *, int, Svar);
FLUENT_EXPORT void DPM_Scalar_Source(Domain *, int, Svar, Svar, Svar, Svar, Svar, Svar);
FLUENT_EXPORT void DPM_Turb_Sources (Domain *domain, int eqn, Svar sv_phi, Svar sv_phi_ap, Svar sv_phi_s );
FLUENT_EXPORT void SV_UpdateDPMSourcesWithIteration(void);
FLUENT_EXPORT void DPM_Reset (void);
FLUENT_EXPORT void DPM_Sync(cxboolean);
FLUENT_EXPORT void DPM_Get_injection_flows(real *sumdt, real *sumnt);
FLUENT_EXPORT void SV_GetSolverParameters (void);
FLUENT_EXPORT void SV_GetSolverRdef (void);
FLUENT_EXPORT void DPM_Free_Thread_Data(Thread *);
FLUENT_EXPORT void DEM_Set_Boundary_Collision_Partner(Thread *thread, char *as_name);
FLUENT_EXPORT void Reset_inDomain_Particles_Summaries(void);
/* @} dpm_interface_solver */

/* @} dpm_interface */

/**
 * @addtogroup dpm_io dpm i/o
 * @{ */

#if ! UDF_COMPILER /* disable for interpreted udfs */

typedef void (* dpm_start_tracks_f) (int nTracks, int64_t *trackIdList);
typedef void (* dpm_insert_track_f) (int64_t   part_id,
                                     int       nPoints,
                                     real *    positions,
                                     int *     cellZones,
                                     size_t *  cellIds,
                                     int       nScalars,
                                     real *    scalars);
typedef void (* dpm_end_tracks_f) ();

typedef struct {

  dpm_start_tracks_f start_tracks;
  dpm_insert_track_f insert_track;
  dpm_end_tracks_f   end_tracks;

} dpm_track_callbacks_t;

FLUENT_EXPORT void DPM_Set_Track_Callbacks(dpm_start_tracks_f start, dpm_insert_track_f insert, dpm_end_tracks_f end);
FLUENT_EXPORT void DPM_Track_Pathlines(int nTracks,
                    real *position,
                    int64_t *cellIndex,
                    int *cellZone,
                    int64_t *particleId,
                    int surfaceId,
                    int maxSteps,
                    int reverse,
                    int nodalValues,
                    int errorControl,
                    real maxError,
                    real lengthScale);
#endif /* ! UDF_COMPILER */

/**
 * @addtogroup dpm_io_file file I/O functions
 * @{ *************************************************************/
FLUENT_EXPORT void Read_Particle_Data(Domain *domain, cxboolean mpi, int fhandle, int *headerbuf, FILE *fd, cxboolean binary, cxboolean double_data);
FLUENT_EXPORT void Write_Particle_Data(Domain *domain, cxboolean mpi, int fhandle, FILE *fd, cxboolean binary);
/* @} dpm_io_file */

/**
 * @addtogroup dpm_io_history writing track history
 * @{ *************************************************************/

/* Moved here from dpm_types.h in order for the
 * subsequent typedef struct ... to work:
 */
#define DPM_NAME_LENGTH 80

/* Moved here from dpm.c in order to
 * use fread_history_header in util/sample.c:
 */
typedef enum
{
  HIST_COL_OPTION = 0,
  HIST_COL_INTEGER32,
  HIST_COL_INTEGER64,
  HIST_COL_FLOAT32,
  HIST_COL_FLOAT64,
  HIST_COL_MAX
} dpm_history_col_type_id_t;

extern FLUENT_EXPORT const char    hist_col_type_name[HIST_COL_MAX][4];
extern FLUENT_EXPORT const char cfdpost_col_type_name[HIST_COL_MAX][16];

typedef struct
{
  /* number of columns in history file */
  int ncol;
  /* number of exported columns */
  int nmap;

  struct dpm_history_column
  {
    char type_name[DPM_NAME_LENGTH];
    dpm_history_col_type_id_t read_type;
    dpm_history_col_type_id_t write_type;

    char name[DPM_NAME_LENGTH];

    /* column index to be exported */
    int map;

    char units[DPM_NAME_LENGTH];

    int min_i32;
    int max_i32;
    int64_t min_i64;
    int64_t max_i64;
    float min_f;
    float max_f;
    double min_d;
    double max_d;

    double *data_d;
    int64_t *data_i64;
    float *data_f;
    int *data_i32;

    cxboolean constant;

  } *col;

  int transient;

  /* maps to certain columns */
  int col_pid;
  int col_step;
  int col_xpos;
  int col_ypos;
  int col_zpos;
  int col_time;

  /* allocated buffer size */
  size_t buffer_size;
} dpm_history_data_t;

FLUENT_EXPORT int fread_history_header(FILE *fp, dpm_history_data_t *hdata);

/* @} dpm_io_history */
/* @} dpm_io */

/**
 * @addtogroup dpm_parallel parallel functions
 * @{ */
FLUENT_EXPORT void init_parallel_injections(Domain *domain);
FLUENT_EXPORT void free_parallel_injections(Domain *domain);
/* @} */


/** model switches institutionalized as functions
 */
cxboolean if_traditional_dpm_collide(void);
cxboolean if_traditional_dpm_collide_no_vof(void);

/*
 * For IO2 library
 */
#define TO_STRING(v) #v
typedef struct dpm_io_t_
{
  int xf_id;
  char name[DPM_NAME_LENGTH];
  int offset;
  int type;
  int mpt_type;
  int size;
  struct dpm_io_t_ *next;
} dpm_io_t;

FLUENT_EXPORT dpm_io_t *Get_DPM_IO_List_Created(void *, int);
FLUENT_EXPORT void Get_DPM_IO_List_Freed(dpm_io_t **);
FLUENT_EXPORT int Get_Particle_Data_Write_Initialized(Domain *);
FLUENT_EXPORT real Get_Flow_Time(void);
FLUENT_EXPORT int Is_Next_Particle_ID_Required(void *);
FLUENT_EXPORT int Get_Particles_Created_For_Reading(void *, int);
FLUENT_EXPORT int Get_Particle_Data_Read_Initialized(Domain *, size_t);
FLUENT_EXPORT int Get_Particle_Data_Read_Finalized(Domain *, void *, int, int , int);

FLUENT_EXPORT void Fix_Unsteady_Particles_Ages(real flow_time);


/* ***** oct tree search <cl_dpm.c> */
FLUENT_EXPORT void DPM_Init_Oct_Tree_Search(void);
FLUENT_EXPORT void DPM_Update_Oct_Tree_Search(void);
FLUENT_EXPORT void DPM_End_Oct_Tree_Search(void);
FLUENT_EXPORT void DPM_Set_Oct_Tree_Init_Flag(cxboolean);
FLUENT_EXPORT void DPM_Print_Search_Statistics(void);
FLUENT_EXPORT int SV_locate_point (real [], CX_Cell_Id *);

#define PARALLEL_DPM ( RP_Get_Boolean("dpm/distributed-memory-parallel?") && \
                       !I_AM_NODE_SERIAL_P )

#define I_DO_DPM ( I_AM_NODE_SERIAL_P || \
                   ( PARALLEL_DPM && I_AM_COMPUTE_NODE_P) || \
                   (!PARALLEL_DPM && I_AM_NODE_HOST_P)  )
#define DPM_NODE_ZERO_OR_HOST ((dpm_par.par_dpm && I_AM_NODE_ZERO_P) || (!dpm_par.par_dpm && I_AM_NODE_HOST_P))

#define PARTICLE_MIGRATION ( PARALLEL_DPM && I_AM_COMPUTE_NODE_P )

#if RP_2D
#  define ADD_SV_VOLUME_2D SV_VOLUME_2D, SV_AREA_2D, 
#else
#  define ADD_SV_VOLUME_2D
#endif

#define DPM_SVAR_NODE(_nv) (dpm_par.svar_iv_index[(int) (_nv)] < 0 ? SV_NULL : dpm_par.interp_vars_node[dpm_par.svar_iv_index[(int) (_nv)]])

#define SV_DPM_LEAN_LIST \
  SV_P, SV_DENSITY,\
  SV_U, SV_V, SV_W,\
  SV_U_G, SV_V_G, SV_W_G, SV_OMEGA_G,\
  SV_U_RG, SV_V_RG, SV_W_RG,\
  SV_FLUX,\
  SV_MU_LAM, SV_MU_T,\
  SV_D, SV_K, SV_O, SV_NUT, \
  SV_N_FILM_HEIGHT, SV_WALL_V, SV_BND_GRID_V,\
  SV_FILM_HEIGHT,SV_FILM_MASS, SV_FILM_MASS_M0, SV_FILM_VELOCITY,\
  SV_FILM_M_IMP, SV_FILM_M_IMP_M1,\
  SV_FILM_P_IMP, SV_FILM_P_IMP_M1,\
  SV_FILM_P_FACE,SV_FILM_P_FACE_G,\
  SV_FILM_TEMP,  SV_FILM_HFLUX,  SV_REFLECT_HFLUX, SV_FILM_H_WALL,\
  SV_FILM_Q_IMP, SV_FILM_Q_IMP_M1,\
  SV_RUU, SV_RVV, SV_RWW, SV_RUV, SV_RVW, SV_RUW,\
  SV_T, SV_H, SV_HEAT_FLUX,\
  SV_P1, SV_DO_IRRAD, SV_DO_IRRAD_OLD, SV_KTC, SV_CP,\
  SV_ABS_COEFF, SV_SCAT_COEFF,\
  SV_FMEAN,SV_FMEAN2,SV_FVAR,SV_FVAR2,SV_PDF_MW,SV_PREMIXC,\
  SV_INERT, SV_INERT_H,\
  SV_Y,\
  EXPAND_NPROB_SV(UFLA_YI),\
  EXPAND_NUDS_SV(UDS),\
  SV_UDM_I,\
  SV_DPM_VIS_0,\
  SV_DPM_BNDRY_DIST,    /* DE29924 */ \
  SV_DPM_DIST, SV_WALL_DIST,        \
  SV_RTMP_0 /* DE59560  */

#define SV_DPM_LIST                             \
  SV_DPM_LEAN_LIST,                             \
    SV_VOLUME,SV_AREA,                          \
    SV_DPM_CELLSTEPS,                           \
    ADD_SV_VOLUME_2D SV_CENTROID,		\
    SV_CENTROID_M1,                             \
    SV_CENTROID_SLIDE
  
#define DPM_SRC_LIST \
  SV_DPMS_MASS,\
  SV_DPMS_MOM_S,\
  SV_DPMS_MOM_AP,\
  SV_DPMS_WSWIRL_S,\
  SV_DPMS_WSWIRL_AP,\
  SV_DPMS_ENERGY,\
  SV_DPMS_ENERGY_AP,\
  SV_DPMS_TKE,\
  SV_DPMS_D,\
  SV_DPMS_O,\
  SV_DPMS_TKE_RUU,\
  SV_DPMS_TKE_RVV,\
  SV_DPMS_TKE_RWW,\
  SV_DPMS_TKE_RUV,\
  SV_DPMS_TKE_RVW,\
  SV_DPMS_TKE_RUW,\
  SV_DPMS_SPECIES,\
  SV_DPMS_SPECIES_AP,\
  EXPAND_NSPE_SV(DPMS_SURFACE_SPECIES),\
  EXPAND_NSPE_SV(DPMS_REACTION_RATE_POST),\
  EXPAND_PER_MATERIAL_SV(DPMS_VAP_PER_MATERIAL),\
  EXPAND_PER_MATERIAL_SV(DPMS_DEVOL_PER_MATERIAL),\
  EXPAND_PER_MATERIAL_SV(DPMS_BURN_PER_MATERIAL),\
  SV_DPMS_PDF_1,\
  SV_DPMS_PDF_2,\
  SV_DPMS_INERT,\
  SV_DPMS_EMISS,\
  SV_DPMS_ABS,\
  SV_DPMS_SCAT,\
  SV_DPMS_BURNOUT,\
  SV_DPMS_CONCENTRATION


#define DPM_SRC_DS_LIST \
  SV_DPMS_DS_MASS,\
  SV_DPMS_DS_MOM_S,\
  SV_DPMS_DS_MOM_AP,\
  SV_DPMS_DS_WSWIRL_S,\
  SV_DPMS_DS_WSWIRL_AP,\
  SV_DPMS_DS_ENERGY,\
  SV_DPMS_DS_ENERGY_AP,\
  SV_DPMS_DS_TKE,\
  SV_DPMS_DS_D,\
  SV_DPMS_DS_O,\
  SV_DPMS_DS_TKE_RUU,\
  SV_DPMS_DS_TKE_RVV,\
  SV_DPMS_DS_TKE_RWW,\
  SV_DPMS_DS_TKE_RUV,\
  SV_DPMS_DS_TKE_RVW,\
  SV_DPMS_DS_TKE_RUW,\
  SV_DPMS_DS_SPECIES,\
  SV_DPMS_DS_SPECIES_AP,\
  EXPAND_NSPE_SV(DPMS_DS_SURFACE_SPECIES),\
  EXPAND_PER_MATERIAL_SV(DPMS_DS_VAP_PER_MATERIAL),\
  EXPAND_PER_MATERIAL_SV(DPMS_DS_DEVOL_PER_MATERIAL),\
  EXPAND_PER_MATERIAL_SV(DPMS_DS_BURN_PER_MATERIAL),\
  SV_DPMS_DS_PDF_1,\
  SV_DPMS_DS_PDF_2,\
  SV_DPMS_DS_INERT,\
  SV_DPMS_DS_EMISS,\
  SV_DPMS_DS_ABS,\
  SV_DPMS_DS_SCAT,\
  SV_DPMS_DS_BURNOUT


#define SV_DPMS_LIST \
  DPM_SRC_LIST, \
  SV_DPMS_EROSION,\
  SV_DPMS_EROSION_MDM,\
  SV_DPMS_ACCRETION, \
  DPM_SRC_DS_LIST, \
  SV_UDM_I

#if _NT
/* windows does provide different 64-bit pointers to locate a position in file */
#define DPM_FTELL _ftelli64
#define DPM_FSEEK _fseeki64
#define DPM_FPOS  __int64
#else
#define DPM_FTELL ftell
#define DPM_FSEEK fseek
#define DPM_FPOS  long
#endif

#endif /* _FLUENT_DPM_H */
