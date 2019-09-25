using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class JetController : MonoBehaviour {
    public float speed;
    Rigidbody rb;
    public int force;
    public float turnrate;
    public float lift,cl,aoa, angleOfattack,ltwr;
    public bool liftstatus;
    public float wing_area = 30f,total_area=(30f+6f+7f+10);
    public float drag, CDs, CDf, CDi=0;
    public float  aoar;
    Vector3 localvelocity,lift_direction, turner;
	// Use this for initialization
	void Start () {
        rb = GetComponent<Rigidbody>();
        force = 0;
        
        

    }

    void calcThrust()
    {
        if (Input.GetKey(KeyCode.W))
            force = force + 1000;
        if (Input.GetKey(KeyCode.S))
            force = force - 1000;
        if (force > 120000)
            force = 120000;
         rb.AddForce(transform.forward * force);
      //  transform.position = transform.position + transform.forward*2;
    }
    void calcDrag()
    {
        CDs = 0.007f;
        aoar = Mathf.Atan2(-localvelocity.y, localvelocity.z);
        CDf = Mathf.Abs(Mathf.Sin((aoar)))*0.75f;
        if (Input.GetKey(KeyCode.Space))
        {
            
            CDi = 0.1f;
        }

        drag = (CDs + CDf + CDi) * 1.22f * total_area * rb.velocity.magnitude * rb.velocity.magnitude / 2;
        CDi = 0;
        rb.AddForce(rb.velocity.normalized* (-1) * drag);
    }
    void calcLift()
    {
        
        aoa = Vector3.Angle(rb.velocity, transform.forward);
       localvelocity = transform.InverseTransformDirection(rb.velocity);
        angleOfattack = Mathf.Atan2(-localvelocity.y, localvelocity.z);
        angleOfattack = angleOfattack  * 360f / (2 * Mathf.PI) ;
      // lvm = localvelocity.magnitude;
        cl = 0.07f * (angleOfattack) + 0.25f;

        
        if (Input.GetKey(KeyCode.Space))
        {
            cl = cl + 0.4f;
           CDi =  0.1f;
        }

        liftstatus = false;
        lift_direction = Vector3.Cross(rb.velocity.normalized, transform.right.normalized);

            if (angleOfattack >= -10 && angleOfattack <= 15)
        {
            liftstatus = true;
           lift = cl * rb.velocity.magnitude * rb.velocity.magnitude * 25.15f;
            ltwr = lift /(rb.mass * 9.81f) ;
       rb.AddForce(lift_direction * lift);
            
        }    //adding lift force
        

    }
    void calcTurn()
    { 
        turnrate = Mathf.Abs(Mathf.Cos(aoar))*rb.velocity.magnitude * rb.velocity.magnitude / 10000f;
      /*  if (angleOfattack > -10 && angleOfattack < 10)
        {
            if (Input.GetKey(KeyCode.UpArrow))
                rb.AddRelativeTorque(Vector3.right * 100000 * turnrate);
            if (Input.GetKey(KeyCode.DownArrow))
                rb.AddRelativeTorque(Vector3.right * (-1) * 100000 * turnrate);

            if (Input.GetKey(KeyCode.RightArrow))
            {

                rb.AddRelativeTorque(Vector3.forward * -500000 * turnrate);

            }




            if (Input.GetKey(KeyCode.LeftArrow))
            {
                
                rb.AddRelativeTorque(Vector3.forward * 500000 * turnrate);

            }
            if (Input.GetKey(KeyCode.A))
                rb.AddRelativeTorque(Vector3.up * -200000 * turnrate);
            if (Input.GetKey(KeyCode.D))
                rb.AddRelativeTorque(Vector3.up * 200000 * turnrate);

        }        
        */
    }
    
    void autobalance()           // Will force aircraft to rotate toward a cruise whenever user leaves the input blank
    {       

    turner = Vector3.Cross(transform.forward, rb.velocity);
        Debug.DrawRay(transform.position, rb.velocity, Color.green, 0.2f);
        Debug.DrawRay(transform.position, transform.forward*100, Color.red, 0.2f);
        rb.AddRelativeTorque(0,turner.y*1000,0);
        if (angleOfattack < -10 || angleOfattack > 15)
        {
            rb.AddRelativeTorque(turner.x*2000, 0,0);
        }



        // cancel sideways velocity 

    }
    // Update is called once per frame
     void Update()
    {
        transform.Rotate(Input.GetAxis("Vertical"),0.0f,-Input.GetAxis("Horizontal")*2) ;
    }
    void FixedUpdate()
    {
        calcThrust();
        calcTurn();
        calcLift();
        calcDrag();
        autobalance();
       // transform.Rotate(1, 0, 0);
        
        
        // to ease out the friction during takeoff
        speed = rb.velocity.magnitude;


    }
}
