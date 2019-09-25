using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {
    public float turnSpeed = 4.0f;
    public GameObject player;
    Vector3 offset ;
	// Use this for initialization
	void Start () {
        offset = new Vector3(player.transform.position.x, player.transform.position.y + 2.0f, player.transform.position.z  -200.0f);
        
    }

    // Update is called once per frame
    void LateUpdate()
    {
        offset = Quaternion.AngleAxis(Input.GetAxis("Mouse X") * turnSpeed, Vector3.up) * offset;

        transform.position = player.transform.position + offset;
        transform.LookAt(player.transform.position);
        
    }
}
