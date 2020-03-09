import json
import requests
import random

def lambda_handler(event, context):

    print(event)
    
    r = requests.post("http://ardueco.altervista.org/insert.php", data=event)
    
    print(r.text)

    if ("RECORD_UPDATE_ERR" in r.text) or ("CONNECTION_ERR" in r.text):
        return {
            'statusCode': 500,
            'body': json.dumps('Lambda NOT OK!')
        }
    else:
        return {
            'statusCode': 200,
            'body': json.dumps('Lambda OK!')
        }

