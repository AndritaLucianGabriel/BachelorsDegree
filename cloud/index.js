// See https://github.com/dialogflow/dialogflow-fulfillment-nodejs
// for Dialogflow fulfillment library docs, samples, and to report issues
'use strict';
 
const functions = require('firebase-functions');
const {WebhookClient} = require('dialogflow-fulfillment');
const {Card, Suggestion} = require('dialogflow-fulfillment');
const {Storage} = require('@google-cloud/storage');
 
process.env.DEBUG = 'dialogflow:debug'; // enables lib debugging statements
 
exports.dialogflowFirebaseFulfillment = functions.https.onRequest((request, response) => {
  const agent = new WebhookClient({ request, response });
  const bucketName = 'licenta_data';
  const fileName = 'example.txt';
  console.log('Dialogflow Request headers: ' + JSON.stringify(request.headers));
  console.log('Dialogflow Request body: ' + JSON.stringify(request.body));
 
  function welcome(agent) {
    agent.add(`Welcome to my agent!`);
  }
 
  function fallback(agent) {
    agent.add(`I didn't understand`);
    agent.add(`I'm sorry, can you try again?`);
  }
  
  function checkBalance(agent) {
    const storage = new Storage();
    const fileContent = 'Hello, this is a sample content!';

    const bucket = storage.bucket(bucketName);
    const file = bucket.file(fileName);
    
    return file.save(fileContent, {
    contentType: 'text/plain',
    })
    .then(() => {
      console.log(`File '${fileName}' has been written to bucket '${bucketName}'`);
      agent.add(`File '${fileName}' has been written to bucket '${bucketName}'`);
    })
    .catch(error => {
      console.error('Error writing file to the bucket:', error);
      agent.add('Error writing file to the bucket: ${error.message}');
    });
  }

  function readFileFromBucket(agent) {
    const storage = new Storage();
    const bucket = storage.bucket(bucketName);
    const file = bucket.file(fileName);

    return file.download()
      .then(data => {
        const fileContent = data[0].toString();
        console.log(`File '${fileName}' content: ${fileContent}`);
        agent.add(`File '${fileName}' content: ${fileContent}`);
      })
      .catch(error => {
        console.error('Error reading file from the bucket:', error);
        agent.add(`Error reading file from the bucket: ${error.message}`);
      });
  }

  function createAccount(agent) {
    const iban = agent.parameters.iban;
    const currency = agent.parameters.currency;
    const sold = agent.parameters.sold;
    
    // Save bank account to the database
    // For example, if you were using Firebase Firestore as the database:
    const db = admin.firestore();
    const bankAccountRef = db.collection('bankAccounts').doc(iban);

    return bankAccountRef.set({
      iban: iban,
      currency: currency,
      sold: sold,
      owner: owner
    })
    .then(() => {
      console.log(`Bank account with IBAN '${iban}' has been created.`);
      agent.add(`Bank account with IBAN '${iban}' has been created for ${owner}.`);
    })
    .catch(error => {
      console.error('Error creating bank account:', error);
      agent.add(`Error creating bank account: ${error.message}`);
    });
  }

  // // Uncomment and edit to make your own intent handler
  // // uncomment `intentMap.set('your intent name here', yourFunctionHandler);`
  // // below to get this function to be run when a Dialogflow intent is matched
  // function yourFunctionHandler(agent) {
  //   agent.add(`This message is from Dialogflow's Cloud Functions for Firebase editor!`);
  //   agent.add(new Card({
  //       title: `Title: this is a card title`,
  //       imageUrl: 'https://developers.google.com/actions/images/badges/XPM_BADGING_GoogleAssistant_VER.png',
  //       text: `This is the body text of a card.  You can even use line\n  breaks and emoji! 💁`,
  //       buttonText: 'This is a button',
  //       buttonUrl: 'https://assistant.google.com/'
  //     })
  //   );
  //   agent.add(new Suggestion(`Quick Reply`));
  //   agent.add(new Suggestion(`Suggestion`));
  //   agent.setContext({ name: 'weather', lifespan: 2, parameters: { city: 'Rome' }});
  // }

  // // Uncomment and edit to make your own Google Assistant intent handler
  // // uncomment `intentMap.set('your intent name here', googleAssistantHandler);`
  // // below to get this function to be run when a Dialogflow intent is matched
  // function googleAssistantHandler(agent) {
  //   let conv = agent.conv(); // Get Actions on Google library conv instance
  //   conv.ask('Hello from the Actions on Google client library!') // Use Actions on Google library
  //   agent.add(conv); // Add Actions on Google library responses to your agent's response
  // }
  // // See https://github.com/dialogflow/fulfillment-actions-library-nodejs
  // // for a complete Dialogflow fulfillment library Actions on Google client library v2 integration sample

  // Run the proper function handler based on the matched Dialogflow intent name
  let intentMap = new Map();
  intentMap.set('Default Welcome Intent', welcome);
  intentMap.set('Default Fallback Intent', fallback);
  intentMap.set('Check Balance', checkBalance);
  intentMap.set('Read File', readFileFromBucket);
  intentMap.set('Create Account', checkBalance);
  // intentMap.set('your intent name here', yourFunctionHandler);
  // intentMap.set('your intent name here', googleAssistantHandler);
  agent.handleRequest(intentMap);
});