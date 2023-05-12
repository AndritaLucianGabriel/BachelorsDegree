// See https://github.com/dialogflow/dialogflow-fulfillment-nodejs
// for Dialogflow fulfillment library docs, samples, and to report issues
'use strict';
 
const functions = require('firebase-functions');
const {WebhookClient} = require('dialogflow-fulfillment');
const {Storage} = require('@google-cloud/storage');
const axios = require('axios');
process.env.DEBUG = 'dialogflow:debug'; // enables lib debugging statements

exports.dialogflowFirebaseFulfillment = functions.https.onRequest((request, response) => {
    const storage = new Storage();
    const agent = new WebhookClient({ request, response });
    console.log('Dialogflow Request headers: ' + JSON.stringify(request.headers));
    console.log('Dialogflow Request body: ' + JSON.stringify(request.body));

    function welcome(agent) {
        agent.add(`Welcome to my agent!`);
    }
    
    function fallback(agent) {
        agent.add(`I didn't understand`);
        agent.add(`I'm sorry, can you try again?`);
    }
    
    // Testing
    async function checkBalance(agent) {
        const fileContent = 'Hello, this is a sample content!';
        const bucketName = 'licenta_data';

        const bucket = storage.bucket(bucketName);
        const fileName = 'example.txt';
        const file = bucket.file(fileName);
        
        await file.save(fileContent, {
        contentType: 'text/plain',
        })
        .then(() => {
        console.log(`File '${fileName}' has been written to bucket '${bucketName}'`);
        agent.add(`File '${fileName}' has been written to bucket '${bucketName}'`);
        })
        .catch(error => {
        console.error('Error writing file to the bucket:', error);
        agent.add(`Error reading file from the bucket: ${error.message}`);
        });
    }

    // Testing
    async function readFileFromBucket(agent) {
        const bucketName = 'licenta_data';
        const fileName = 'example.txt';
        const bucket = storage.bucket(bucketName);
        const file = bucket.file(fileName);

        await file.download()
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

    async function createAccount(agent) {
        const iban = agent.parameters.iban;
        const currency = agent.parameters.currency;
        const sold = agent.parameters.sold;
      
        const bucketName = 'bank-accounts';
        const fileName = iban + '.json';
      
        // Initialize Google Cloud Storage
        const bucket = storage.bucket(bucketName);
        const file = bucket.file(fileName);
      
        try {
          // Check if the file exists
          const [exists] = await file.exists();
      
          if (!exists) {
            // Prepare bank account data as a JSON string
            const bankAccountData = JSON.stringify({
              iban: iban,
              currency: currency,
              sold: sold,
            });
      
            // Save bank account data to Google Cloud Storage
            await file.save(bankAccountData, {
              contentType: 'application/json',
            });
      
            console.log(`Bank account with IBAN '${iban}' has been created.`);
            agent.add(`Bank account with IBAN '${iban}' has been created.`);
          } else {
            console.log(`Bank account with IBAN '${iban}' already exists.`);
            agent.add(`Bank account with IBAN '${iban}' already exists.`);
          }
        } catch (error) {
          console.error('Error creating bank account:', error);
          agent.add(`Error creating bank account: ${error.message}`);
        }
    }

    async function transferMoney(agent) {
        const sourceIban = agent.parameters.sourceIban;
        const destinationIban = agent.parameters.destinationIban;
        const amount = agent.parameters.amount;
        
        const bucketName = 'bank-accounts';
        const bucket = storage.bucket(bucketName);
      
        try {
          // Read source and destination account data
          const [sourceData, destinationData] = await Promise.all([
            bucket.file(sourceIban + '.json').download(),
            bucket.file(destinationIban + '.json').download()
          ]);
      
          const sourceAccount = JSON.parse(sourceData[0]);
          const destinationAccount = JSON.parse(destinationData[0]);
      
          // Check if the source account has enough balance
          if (sourceAccount.sold < amount) {
            agent.add(`Insufficient balance in the source account with IBAN '${sourceIban}'.`);
            return;
          }
      
          // Get conversion rate between source and destination currencies
          const conversionRate = await getConversionRate(sourceAccount.currency, destinationAccount.currency);
      
          // Calculate the transferred amount in the destination currency
          const convertedAmount = amount * conversionRate;
      
          // Update account balances
          sourceAccount.sold = parseFloat(sourceAccount.sold - amount);
          destinationAccount.sold = parseFloat(destinationAccount.sold + convertedAmount);
      
          // Save updated account data
          await Promise.all([
            bucket.file(sourceIban + '.json').save(JSON.stringify(sourceAccount), { contentType: 'application/json' }),
            bucket.file(destinationIban + '.json').save(JSON.stringify(destinationAccount), { contentType: 'application/json' })
          ]);
      
          agent.add(`Transferred ${amount} ${sourceAccount.currency} from account with IBAN '${sourceIban}' to account with IBAN '${destinationIban}'. The destination account received ${convertedAmount.toFixed(2)} ${destinationAccount.currency}.`);
        } catch (error) {
          console.error('Error transferring money:', error);
          agent.add(`Error transferring money: ${error.message}`);
        }
    }

    async function addAmount(agent) {
        const iban = agent.parameters.iban;
        const amount = agent.parameters.amount;
        const currency = agent.parameters.currency;
      
        const bucketName = 'bank-accounts';
        const fileName = iban + '.json';
      
        // Initialize Google Cloud Storage
        const bucket = storage.bucket(bucketName);
        const file = bucket.file(fileName);

        // Check if the file exists
        const [exists] = await file.exists();
    
        if (exists) {
            try {
                // Read account data
                const [accountData] = await file.download();
                const account = JSON.parse(accountData.toString());
                console.log("account: " + account);
                // Get conversion rate between source and destination currencies
                const conversionRate = await getConversionRate(account.currency, currency);
                console.log("ConversionRate: " + conversionRate);
                // Calculate the transferred amount in the destination currency
                const convertedAmount = amount * conversionRate;
                console.log("convertedAmount: " + convertedAmount);
                // Update account balance
                account.sold = parseFloat(account.sold + convertedAmount);
                console.log("account.sold: " + account.sold);
                // Save updated account data
                await file.save(JSON.stringify(account), { contentType: 'application/json' });
            
                agent.add(`Added ${amount} ${account.currency} to the account with IBAN '${iban}'. The new balance is ${account.sold} ${account.currency}.`);
            } catch (error) {
                console.error('Error adding amount to account:', error);
                agent.add(`Error adding amount to account: ${error.message}`);
            }
        }
        else {
            console.log(`Bank account with IBAN '${iban}' doesn't exist.`);
            agent.add(`Bank account with IBAN '${iban}' doesn't exist.`);
        }
    }

    async function getConversionRate(sourceCurrency, destinationCurrency) {
        const apiKey = 'Hx5QBKAESeOlThw7d5R0kysnrl2jUu1zpZj3ociN';
        const url = `https://api.freecurrencyapi.ro/v1/convert?from=${sourceCurrency}&to=${destinationCurrency}&api_key=${apiKey}`;
      
        try {
          const response = await axios.get(url);
          const conversionRate = response.data.rate;
          return conversionRate;
        } catch (error) {
          console.error('Error retrieving conversion rate:', error);
          throw error;
        }
    }

    async function createBucketIfNotExists(bucketName) {
        try {
        // Check if the bucket exists
        const [buckets] = await storage.getBuckets();
        const bucketExists = buckets.some(bucket => bucket.name === bucketName);
    
        // If the bucket doesn't exist, create it
        if (!bucketExists) {
            await storage.createBucket(bucketName);
            console.log(`Bucket '${bucketName}' has been created.`);
        } else {
            console.log(`Bucket '${bucketName}' already exists.`);
        }
        } catch (error) {
        console.error('Error creating bucket:', error);
        }
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
    intentMap.set('Create Account', createAccount);
    intentMap.set('Transfer Money', transferMoney);
    intentMap.set('Deposit', addAmount);
    // intentMap.set('your intent name here', yourFunctionHandler);
    // intentMap.set('your intent name here', googleAssistantHandler);
    agent.handleRequest(intentMap);
});