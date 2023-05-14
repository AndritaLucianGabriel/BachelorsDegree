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
    
    async function checkBalance(agent) {
        const iban = agent.parameters.iban;
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

                console.log(`The account with IBAN '${iban}' has a balance of: ${account.sold} ${account.currency}.`);
                agent.add(`The account with IBAN '${iban}' has a balance of: ${account.sold} ${account.currency}.`);
            } catch(error) {
                console.error('Error checking the balance:', error);
                agent.add(`Error checking the balance: ${error.message}`);
            }
        }
        else {
            console.log(`Bank account with IBAN '${iban}' doesn't exist.`);
            agent.add(`Bank account with IBAN '${iban}' doesn't exist.`);
        }
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
        const currency = agent.parameters.currency;
        
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
        
            if(currency === sourceAccount.currency || currency === '') {
                // Check if the source account has enough balance
                console.log("sourceAccount.sold: " + sourceAccount.sold);
                console.log("amount: " + amount);
                if (parseFloat(sourceAccount.sold) < parseFloat(amount)) {
                    agent.add(`Insufficient balance in the source account with IBAN '${sourceIban}'.`);
                    return;
                }

                // Get conversion rate between source and destination currencies
                const conversionRate = await getConversionRate(sourceAccount.currency, destinationAccount.currency);

                // Calculate the transferred amount in the destination currency
                const convertedAmount = amount * conversionRate;

                // Update account balances
                sourceAccount.sold = parseFloat(sourceAccount.sold) - parseFloat(amount);
                sourceAccount.sold = Number(sourceAccount.sold).toFixed(2);

                destinationAccount.sold = parseFloat(destinationAccount.sold) + parseFloat(convertedAmount);
                destinationAccount.sold = Number(destinationAccount.sold).toFixed(2);

                // Save updated account data
                await Promise.all([
                        bucket.file(sourceIban + '.json').save(JSON.stringify(sourceAccount), { contentType: 'application/json' }),
                        bucket.file(destinationIban + '.json').save(JSON.stringify(destinationAccount), { contentType: 'application/json' })
                ]);
                console.log(`Transferred ${amount} ${sourceAccount.currency} from account with IBAN '${sourceIban}' to account with IBAN '${destinationIban}'. The destination account received ${convertedAmount.toFixed(2)} ${destinationAccount.currency}.`);
                agent.add(`Transferred ${amount} ${sourceAccount.currency} from account with IBAN '${sourceIban}' to account with IBAN '${destinationIban}'. The destination account received ${convertedAmount.toFixed(2)} ${destinationAccount.currency}.`);
            }
            else if(currency === destinationAccount.currency) {    
                    // Get conversion rate between source and destination currencies    
                    const conversionRate = await getConversionRate(destinationAccount.currency, sourceAccount.currency);
                    
                    // Calculate the transferred amount in the destination currency
                    const convertedAmount = amount * conversionRate;
                    
                    if (parseFloat(sourceAccount.sold) < parseFloat(convertedAmount)) {
                        agent.add(`Insufficient balance in the source account with IBAN '${sourceIban}'.`);
                        return;
                    }
    
                    // Update account balances
                    sourceAccount.sold = parseFloat(sourceAccount.sold) - parseFloat(convertedAmount);
                    sourceAccount.sold = Number(sourceAccount.sold).toFixed(2);

                    destinationAccount.sold = parseFloat(destinationAccount.sold) + parseFloat(amount);
                    destinationAccount.sold = Number(destinationAccount.sold).toFixed(2);

                    // Save updated account data
                    await Promise.all([
                            bucket.file(sourceIban + '.json').save(JSON.stringify(sourceAccount), { contentType: 'application/json' }),
                            bucket.file(destinationIban + '.json').save(JSON.stringify(destinationAccount), { contentType: 'application/json' })
                    ]);
                    console.log(`Transferred ${convertedAmount.toFixed(2)} ${sourceAccount.currency} from account with IBAN '${sourceIban}' to account with IBAN '${destinationIban}'. The destination account received ${amount} ${destinationAccount.currency}.`);
                    agent.add(`Transferred ${convertedAmount.toFixed(2)} ${sourceAccount.currency} from account with IBAN '${sourceIban}' to account with IBAN '${destinationIban}'. The destination account received ${amount} ${destinationAccount.currency}.`);
            }
            else {
                console.log(`Invalid currency for transfer. It has to be either ${sourceAccount.currency} or ${destinationAccount.currency}.`);
                agent.add(`Invalid currency for transfer. It has to be either ${sourceAccount.currency} or ${destinationAccount.currency}.`);
            }
            
        } catch (error) {
            console.error('Error transferring money:', error);
            agent.add(`Error transferring money: ${error.message}`);
        }
    }

    async function addAmount(agent) {
        const iban = agent.parameters.iban;
        const amount = agent.parameters.amount;
        let currency = agent.parameters.currency;
      
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

                // Get conversion rate between source and destination currencies
                let conversionRate = 1.0;
                console.log("currency: " + currency);
                console.log("account.currency: " + account.currency);
                if(currency !== '') {
                    conversionRate = await getConversionRate(currency, account.currency);
                } else {
                    // For debugging purposes
                    currency = account.currency;                    
                }
                console.log("ConversionRate: " + conversionRate);
                // Calculate the transferred amount in the destination currency
                const convertedAmount = amount * conversionRate;
                console.log("convertedAmount: " + convertedAmount);
                // Update account balance
                console.log("before account.sold: " + account.sold);
                account.sold = parseFloat(account.sold) + parseFloat(convertedAmount);
                console.log("after adding account.sold: " + account.sold);

                account.sold = Number(account.sold).toFixed(2);
                console.log("after rounded account.sold: " + account.sold);
                // Save updated account data
                await file.save(JSON.stringify(account), { contentType: 'application/json' });
            
                console.log(`Added ${amount} ${currency} to the account with IBAN '${iban}'. The new balance is ${account.sold} ${account.currency}.`);
                agent.add(`Added ${amount} ${currency} to the account with IBAN '${iban}'. The new balance is ${account.sold} ${account.currency}.`);
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

    async function withdrawAmount(agent) {
        const iban = agent.parameters.iban;
        const amount = agent.parameters.amount;
        let currency = agent.parameters.currency;
      
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

                // Get conversion rate between source and destination currencies
                let conversionRate = 1.0;
                console.log("currency: " + currency);
                console.log("account.currency: " + account.currency);
                if(currency !== '') {
                    conversionRate = await getConversionRate(currency, account.currency);
                } else {
                    // For debugging purposes
                    currency = account.currency;                    
                }
                console.log("ConversionRate: " + conversionRate);
                // Calculate the transferred amount in the destination currency
                const convertedAmount = amount * conversionRate;
                console.log("convertedAmount: " + convertedAmount);
                if (parseFloat(account.sold) < parseFloat(convertedAmount)) {
                    agent.add(`Insufficient balance in the source account with IBAN '${account.iban}'.`);
                    return;
                }
                // Update account balance
                console.log("before account.sold: " + account.sold);
                account.sold = parseFloat(account.sold) - parseFloat(convertedAmount);
                console.log("after withdraw account.sold: " + account.sold);

                account.sold = Number(account.sold).toFixed(2);
                console.log("after rounded account.sold: " + account.sold);
                // Save updated account data
                await file.save(JSON.stringify(account), { contentType: 'application/json' });
            
                console.log(`Withdrew ${amount} ${currency} from the account with IBAN '${iban}'. The new balance is ${account.sold} ${account.currency}.`);
                agent.add(`Withdrew ${amount} ${currency} from the account with IBAN '${iban}'. The new balance is ${account.sold} ${account.currency}.`);
            } catch (error) {
                console.error('Error withdrawing amount to account:', error);
                agent.add(`Error withdrawing amount to account: ${error.message}`);
            }
        }
        else {
            console.log(`Bank account with IBAN '${iban}' doesn't exist.`);
            agent.add(`Bank account with IBAN '${iban}' doesn't exist.`);
        }
    }

    async function closeAccount(agent) {
        const iban = agent.parameters.iban;
        const bucketName = 'bank-accounts';
        const fileName = iban + '.json';

        // Initialize Google Cloud Storage
        const bucket = storage.bucket(bucketName);
        const file = bucket.file(fileName);

        // Check if the file exists
        const [exists] = await file.exists();
    
        if (exists) {
            await file.delete();

            console.log(`The bank account '${iban}' has been closed!`);
            agent.add(`The bank account '${iban}' has been closed!`);
        }
        else {
            console.log(`Bank account with IBAN '${iban}' doesn't exist.`);
            agent.add(`Bank account with IBAN '${iban}' doesn't exist.`);
        }
    }

    async function listAccounts(agent) {
        const bucketName = 'bank-accounts';
    
        // Initialize Google Cloud Storage
        const bucket = storage.bucket(bucketName);
    
        // List all files in the bucket
        const [files] = await bucket.getFiles();
    
        if (files.length > 0) {
            let ibanList = [];
            files.forEach(file => {
                const iban = file.name.replace('.json', '');
                ibanList.push(iban);
            });
            
            if(files.length != 1) {
                console.log(`Found ${ibanList.length} bank accounts. IBANs: ${ibanList.join(', ')}`);
                agent.add(`Found ${ibanList.length} bank accounts. IBANs: ${ibanList.join(', ')}`);
            }
            else {
                console.log(`Found ${ibanList.length} bank account. IBAN: ${ibanList.join(', ')}`);
                agent.add(`Found ${ibanList.length} bank account. IBAN: ${ibanList.join(', ')}`);
            }
        } else {
            console.log('No bank accounts found.');
            agent.add('No bank accounts found.');
        }
    }


    async function getConversionRate(sourceCurrency, destinationCurrency) {
        const apiKey = 'Hx5QBKAESeOlThw7d5R0kysnrl2jUu1zpZj3ociN';
        const url = `https://api.freecurrencyapi.com/v1/latest?apikey=${apiKey}&base_currency=${sourceCurrency}&currencies=${destinationCurrency}`;
        console.log("URL: " + url);
        try {
            const response = await axios.get(url);
            console.log("response.data: " + JSON.stringify(response.data));
            const conversionData = response.data['data'];
            console.log("conversionData: " + JSON.stringify(conversionData));
            const conversionRate = conversionData[destinationCurrency];
            console.log("conversionRate: " + conversionRate);
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
    intentMap.set('Create Account', createAccount);
    intentMap.set('Transfer Money', transferMoney);
    intentMap.set('Deposit', addAmount);
    intentMap.set('Close Account', closeAccount);
    intentMap.set('Withdraw', withdrawAmount);
    intentMap.set('List Accounts', listAccounts);
    // intentMap.set('your intent name here', yourFunctionHandler);
    // intentMap.set('your intent name here', googleAssistantHandler);
    agent.handleRequest(intentMap);
});